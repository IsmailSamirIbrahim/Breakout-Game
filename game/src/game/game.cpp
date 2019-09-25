#include "game/game.h"
#include "component/background.h"

#include <glm/gtc/matrix_transform.hpp>

#include <resource_manager/resource_manager.h>

#include <string>
#include <assert.h>

using namespace std;
using namespace glm;
using namespace bko;

namespace bko
{
#define PLAYER_PADDLE_SIZE vec2{100.0, 20.0f}
#define PLAYER_PADDLE_VELOCITY vec2 {500.0f, 0.0f}
#define BALL_RADIUS 12.5f
#define BALL_VELOCITY vec2 {100.0f, -350.0f}

	static Resource_Manager rm = IResource_Manager::get_instance();

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
	
	DIRECTION
	_game_vector_direction(vec2 target)
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

	//Helper functions
	inline static void
	_game_ball_update(Game& self, GLfloat delta_time)
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

	inline static Collision_Info
	_game_collision_check(const Ball& ball, const Brick& brick)
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

		return Collision_Info{ _game_vector_direction(difference), res, difference };
	}

	inline static Collision_Info
	_game_collision_check(const Ball& ball, const Player_Paddle& player)
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

		return Collision_Info{ _game_vector_direction(difference), res, difference };
	}

	inline static void
	_game_do_collisions(Game& self)
	{
		for (Brick& brick : self.levels[self.current_level - 1].bricks)
			if (!brick.is_destroyed)
			{
				Collision_Info collision_info = _game_collision_check(self.ball, brick);
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
				
				collision_info = _game_collision_check(self.ball, self.player);
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
	
	
	// API
	Game
	game_new(GLsizei width, GLsizei height)
	{
		Game self{};

		self.state = Game::STATE_ACTIVE;
		self.current_level = 1;
		self.player = Player_Paddle{};
		self.ball = Ball{};
		self.window = window_new(width, height);
		self.sprite_renderer = nullptr;

		return self;
	}

	void
	game_free(Game self)
	{
		for(auto level : self.levels)
			destruct(level);

		destruct(self.player);
		destruct(self.ball);
		destruct(self.window);
		destruct(self.sprite_renderer);
	}

	void
	game_init(Game& self)
	{
		// load shaders
		string vshader_path = string(SHADER_DIR) + string("/shaders/sprite.vs");
		string fshader_path = string(SHADER_DIR) + string("/shaders/sprite.fs");
		resource_manager_load_program(rm, vshader_path.c_str(), fshader_path.c_str(), "sprite");

		// load textures
		string image_path = std::string(IMAGE_DIR) + std::string("/images/background.jpg");
		resource_manager_load_texture(rm, image_path.c_str(), "background");

		image_path = std::string(IMAGE_DIR) + std::string("/images/block.png");
		resource_manager_load_texture(rm, image_path.c_str(), "block");

		image_path = std::string(IMAGE_DIR) + std::string("/images/block_solid.png");
		resource_manager_load_texture(rm, image_path.c_str(), "block_solid");

		image_path = std::string(IMAGE_DIR) + std::string("/images/paddle.png");
		resource_manager_load_texture(rm, image_path.c_str(), "paddle");

		image_path = std::string(IMAGE_DIR) + std::string("/images/ball.png");
		resource_manager_load_texture(rm, image_path.c_str(), "ball");

		// load levels
		string level_path = std::string(LEVEL_DIR) + std::string("/levels/standard.lvl");
		Level level = level_new(self.window.width, self.window.height * 0.5, level_path.c_str());
		self.levels.push_back(level);

		level_path = std::string(LEVEL_DIR) + std::string("/levels/few_small_gaps.lvl");
		level = level_new(self.window.width, self.window.height * 0.5, level_path.c_str());
		self.levels.push_back(level);

		level_path = std::string(LEVEL_DIR) + std::string("/levels/space_invader.lvl");
		level = level_new(self.window.width, self.window.height * 0.5, level_path.c_str());
		self.levels.push_back(level);

		level_path = std::string(LEVEL_DIR) + std::string("/levels/bounce_galore.lvl");
		level = level_new(self.window.width, self.window.height * 0.5, level_path.c_str());
		self.levels.push_back(level);

		// configure shaders
		mat4 projection = ortho(0.0f, static_cast<GLfloat>(self.window.width), static_cast<GLfloat>(self.window.height), 0.0f, -1.0f, 1.0f);
		Program program = resource_manager_program(rm, "sprite");
		program_use(program);
		program_int_set(program, "image", 0);
		program_mat4f_set(program, "projection", projection);
		
		// Set render-specific controls
		self.sprite_renderer = sprite_renderer_new(program);

		// player initialization
		Texture texture = resource_manager_texture(rm, "paddle");
		self.player = player_paddle_new(texture,
			vec2{ self.window.width / 2 - PLAYER_PADDLE_SIZE.x / 2, self.window.height - PLAYER_PADDLE_SIZE.y },
			PLAYER_PADDLE_SIZE,
			PLAYER_PADDLE_VELOCITY
		);

		// ball initialization
		texture = resource_manager_texture(rm, "ball");
		vec2 ball_pos = self.player.sprite.position + vec2{ self.player.sprite.size.x / 2 - 12.5f, -1 * BALL_RADIUS * 2 };
		self.ball = ball_new(texture, ball_pos, BALL_VELOCITY, BALL_RADIUS, GL_TRUE);
	}

	inline static void
	game_process_input(Game& self, GLfloat delta_time)
	{
		if (self.state == Game::STATE_ACTIVE)
		{
			if (glfwGetKey(self.window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(self.window.handle, true);

			if (glfwGetKey(self.window.handle, GLFW_KEY_A) == GLFW_PRESS)
				self.keys[GLFW_KEY_A] = GL_TRUE;

			if (glfwGetKey(self.window.handle, GLFW_KEY_A) == GLFW_RELEASE)
				self.keys[GLFW_KEY_A] = GL_FALSE;

			if (glfwGetKey(self.window.handle, GLFW_KEY_D) == GLFW_PRESS)
				self.keys[GLFW_KEY_D] = GL_TRUE;

			if (glfwGetKey(self.window.handle, GLFW_KEY_D) == GLFW_RELEASE)
				self.keys[GLFW_KEY_D] = GL_FALSE;

			if (glfwGetKey(self.window.handle, GLFW_KEY_SPACE) == GLFW_PRESS)
				self.keys[GLFW_KEY_SPACE] = GL_TRUE;

			if (glfwGetKey(self.window.handle, GLFW_KEY_SPACE) == GLFW_RELEASE)
				self.keys[GLFW_KEY_SPACE] = GL_FALSE;

			GLfloat velocity = self.player.velocity.x * delta_time;
			if (self.keys[GLFW_KEY_A])
			{
				if (self.player.sprite.position.x >= 0)
				{
					self.player.sprite.position.x -= velocity;
					if (self.ball.is_stuck)
						self.ball.sprite.position.x -= velocity;
				}
			}
			if (self.keys[GLFW_KEY_D])
			{
				if (self.player.sprite.position.x <= self.window.width - self.player.sprite.size.x)
				{
					self.player.sprite.position.x += velocity;
					if (self.ball.is_stuck)
						self.ball.sprite.position.x += velocity;
				}

			}
			if (self.keys[GLFW_KEY_SPACE])
			{
				self.ball.is_stuck = GL_FALSE;
			}
		}
	}

	inline static void
	game_update(Game& self, GLfloat delta_time)
	{
		_game_ball_update(self, delta_time);
		_game_do_collisions(self);
	}

	inline static void
	game_render(Game self)
	{
		if (self.state == Game::STATE_ACTIVE)
		{
			// render background
			Texture bkg = resource_manager_texture(rm, "background");
			Background background = background_new(bkg, vec2{ 0.0f, 0.0f }, vec2{ self.window.width, self.window.height });
			sprite_renderer_render(self.sprite_renderer, background.sprite);

			// render level
			for (auto brick : self.levels[self.current_level - 1].bricks)
				if (!brick.is_destroyed)
					sprite_renderer_render(self.sprite_renderer, brick.sprite);

			//render player paddle
			sprite_renderer_render(self.sprite_renderer, self.player.sprite);

			//render ball
			sprite_renderer_render(self.sprite_renderer, self.ball.sprite);
		}
	}

	void
	game_run(Game self)
	{
		float current_frame;
		float delta_time = 0.0f;
		float last_frame = 0.0f;
		// render loop
		while (!glfwWindowShouldClose(self.window.handle))
		{
			current_frame = glfwGetTime();
			delta_time = current_frame - last_frame;
			last_frame = current_frame;
			// input
			game_process_input(self, delta_time);

			//update
			game_update(self, delta_time);

			// render stuff
			game_render(self);

			// swap buffers
			glfwSwapBuffers(self.window.handle);

			// poll events
			glfwPollEvents();
		}
	}

}