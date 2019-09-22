#include "game/game_object.h"

using namespace glm;

namespace bko
{
	Game_Object
	game_object_background_new(Texture texture, vec2 position, vec2 size)
	{
		Game_Object self{};

		self.kind = Game_Object::KIND_BACKGROUND;
		self.texture = texture;
		self.color = vec3{ 1.0f, 1.0f, 1.0f };
		self.position = position;
		self.size = size;

		return self;
	}

	Game_Object
	game_object_brick_new(Texture texture, vec3 color, vec2 position, vec2 size, GLboolean is_solid, GLboolean is_destroyed)
	{
		Game_Object self{};

		self.kind = Game_Object::KIND_BRICK;
		self.texture = texture;
		self.color = color;
		self.position = position;
		self.size = size;
		self.brick.is_solid = is_solid;
		self.brick.is_destroyed = is_destroyed;

		return self;
	}

	Game_Object
	game_object_player_paddle_new(Texture texture, vec2 position, vec2 size, vec2 velocity)
	{
		Game_Object self{};

		self.kind = Game_Object::KIND_PLAYER_PADDLE;
		self.texture = texture;
		self.color = vec3{ 1.0f, 1.0f, 1.0f };
		self.position = position;
		self.size = size;
		self.player_paddle.velocity = velocity;

		return self;
	}

	Game_Object
	game_object_ball_new(Texture texture, vec2 position, GLfloat radius, vec2 velocity, GLboolean is_stuck)
	{
		Game_Object self{};

		self.kind = Game_Object::KIND_BALL;
		self.texture = texture;
		self.color = vec3{ 1.0f, 1.0f, 1.0f };
		self.position = position;
		self.size = vec2{ radius * 2, radius * 2 };
		self.ball.radius = radius;
		self.ball.velocity = velocity;
		self.ball.is_stuck = is_stuck;

		return self;
	}

	void
	game_object_free(Game_Object self)
	{
		destruct(self.texture);
	}
}
