#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "game/game.h"

#include "component/brick.h"
#include "component/ball.h"
#include "component/player_paddle.h"

using namespace glm;

namespace bko
{
#define BALL_VELOCITY vec2 {100.0f, -350.0f}

	enum DIRECTION
	{
		DIRECTION_UP,
		DIRECTION_RIGHT,
		DIRECTION_DOWN,
		DIRECTION_LEFT
	};

	struct Collision_Info
	{
		DIRECTION direction;
		GLboolean is_collid;
		vec2 difference;
	};

	//Helper functions
	inline static void
	collision_ball_edge_detection(Game& self, GLfloat delta_time)
	{
		if (!self.ball.is_stuck)
		{
			self.ball.sprite.position += self.ball.velocity * delta_time;

			if (self.ball.sprite.position.x <= 0.0f)
			{
				self.ball.velocity.x *= -1;
				self.ball.sprite.position.x = 0.0f;
			}
			else if (self.ball.sprite.position.x + self.ball.sprite.size.x >= self.window.width)
			{
				self.ball.velocity.x *= -1;
				self.ball.sprite.position.x = self.window.width - self.ball.sprite.size.x;
			}
			if (self.ball.sprite.position.y <= 0.0f)
			{
				self.ball.velocity.y *= -1;
				self.ball.sprite.position.y = 0.0f;
			}
		}
	}

	DIRECTION
	_vector_direction(vec2 target)
	{
		vec2 compass[] = {
			vec2(0.0f, 1.0f),	// up
			vec2(1.0f, 0.0f),	// right
			vec2(0.0f, -1.0f),	// down
			vec2(-1.0f, 0.0f)	// left
		};

		GLfloat max = 0.0f;
		GLuint best_match = -1;
		for (GLuint i = 0; i < 4; i++)
		{
			GLfloat dot_product = dot(normalize(target), compass[i]);
			if (dot_product > max)
			{
				max = dot_product;
				best_match = i;
			}
		}
		return (DIRECTION)best_match;
	}

	inline static Collision_Info
	collision_ball_brick_detection(const Ball& ball, const Brick& brick)
	{
		// Get center point of the circle
		vec2 ball_center = vec2{ ball.sprite.position + ball.radius };

		// Calculate brick info (center, half-extents)
		vec2 half_extents = vec2{ brick.sprite.size.x / 2.0f, brick.sprite.size.y / 2.0 };
		vec2 brick_center = vec2{ brick.sprite.position.x + half_extents.x, brick.sprite.position.y + half_extents.y };
		// Get difference vector between both centers
		vec2 diff_center = ball_center - brick_center;
		vec2 clamped = clamp(diff_center, -half_extents, half_extents);

		//Add clamped value to brick_center and we get the value of box closest to circle
		vec2 closest_point = brick_center + clamped;

		vec2 difference = closest_point - ball_center;

		GLboolean res = length(difference) < ball.radius;

		return Collision_Info{ _vector_direction(difference), res, difference };
	}

	inline static Collision_Info
	collision_ball_player_detection(const Ball& ball, const Player_Paddle& player)
	{
		// Get center point of the circle
		vec2 ball_center = vec2{ ball.sprite.position + ball.radius };

		// Calculate player info (center, half-extents)
		vec2 half_extents = vec2{ player.sprite.size.x / 2.0f, player.sprite.size.y / 2.0 };
		vec2 player_center = vec2{ player.sprite.position.x + half_extents.x, player.sprite.position.y + half_extents.y };
		// Get difference vector between both centers
		vec2 diff_center = ball_center - player_center;
		vec2 clamped = clamp(diff_center, -half_extents, half_extents);

		//Add clamped value to player_center and we get the value of box closest to circle
		vec2 closest_point = player_center + clamped;

		vec2 difference = closest_point - ball_center;

		GLboolean res = length(difference) < ball.radius;

		return Collision_Info{ _vector_direction(difference), res, difference };
	}

	inline static void
	collision_detection(Game& self, GLfloat delta_time)
	{
		collision_ball_edge_detection(self, delta_time);

		for (Brick& brick : self.levels[self.current_level - 1].bricks)
			if (!brick.is_destroyed)
			{
				Collision_Info collision_info = collision_ball_brick_detection(self.ball, brick);
				if (collision_info.is_collid)
				{
					// Destroy block if not solid
					if (!brick.is_solid)
						brick.is_destroyed = GL_TRUE;

					// Collision resolution
					DIRECTION direction = collision_info.direction;
					vec2 difference = collision_info.difference;
					if (direction == DIRECTION_LEFT || direction == DIRECTION_RIGHT)
					{
						// Reverse velocity
						self.ball.velocity.x *= -1;
						// Reposition
						GLfloat penetration = self.ball.radius - abs(difference.x);
						if (direction == DIRECTION_LEFT)
							self.ball.sprite.position.x += penetration;
						else
							self.ball.sprite.position.x -= penetration;
					}
					else
					{
						// Reverse velocity
						self.ball.velocity.y *= -1;
						// Reposition
						GLfloat penetration = self.ball.radius - abs(difference.y);
						if (direction == DIRECTION_UP)
							self.ball.sprite.position.y -= penetration;
						else
							self.ball.sprite.position.y += penetration;
					}
				}

				collision_info = collision_ball_player_detection(self.ball, self.player);
				if (!self.ball.is_stuck && collision_info.is_collid)
				{
					// Check where it hit the board, and change velocity based on where it hit the board
					GLfloat center_paddle = self.player.sprite.position.x + self.player.sprite.size.x / 2;
					GLfloat distance = (self.ball.sprite.position.x + self.ball.radius) - center_paddle;
					GLfloat percentage = distance / (self.player.sprite.size.x / 2);
					// Then move accordingly
					GLfloat strength = 2.0f;
					vec2 old_velocity = self.ball.velocity;
					self.ball.velocity.x = BALL_VELOCITY.x * percentage * strength;
					self.ball.velocity.y = -1 * abs(self.ball.velocity.y);
					self.ball.velocity = normalize(self.ball.velocity) * length(old_velocity);
				}
			}
	}

} //namespace end