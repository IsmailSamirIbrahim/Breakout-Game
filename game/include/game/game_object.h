#pragma once

#include<game/Exports.h>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <texture/texture.h>

namespace bko
{
	struct Game_Object
	{
		enum KIND
		{
			KIND_NONE,
			KIND_BACKGROUND,
			KIND_BRICK,
			KIND_PLAYER_PADDLE,
			KIND_BALL
		};

		KIND kind;

		//background or somthing else
		Texture texture;
		glm::vec3 color;
		glm::vec2 position;
		glm::vec2 size;

		union
		{
			struct
			{
				GLboolean is_solid;
				GLboolean is_destroyed;
			}brick;

			struct
			{
				glm::vec2 velocity;
			}player_paddle;

			struct
			{
				GLfloat radius;
				glm::vec2 velocity;
				GLboolean is_stuck;
			}ball;
		};
	};

	GAME_EXPORT Game_Object
	game_object_background_new(Texture texture, glm::vec2 position, glm::vec2 size);

	GAME_EXPORT Game_Object
	game_object_brick_new(Texture texture, glm::vec3 color, glm::vec2 position, glm::vec2 size, GLboolean is_solid, GLboolean is_destroyed);

	GAME_EXPORT Game_Object
	game_object_player_paddle_new(Texture texture, glm::vec2 position, glm::vec2 size, glm::vec2 velocity);

	GAME_EXPORT Game_Object
	game_object_ball_new(Texture texture, glm::vec2 position, GLfloat radius, glm::vec2 velocity, GLboolean is_stuck);

	GAME_EXPORT void
	game_object_free(Game_Object self);

	inline static void
	destruct(Game_Object self)
	{
		game_object_free(self);
	}

} //namespace end