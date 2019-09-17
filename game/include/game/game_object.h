#pragma once

#include<game/Exports.h>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <texture/texture.h>

namespace bko
{
	struct Game_Object
	{
		Texture texture;
		glm::vec3 color;
		glm::vec2 position;
		glm::vec2 size;;
		glm::vec2 velocity;
		GLfloat rotation_angle;
		GLboolean is_solid;
		GLboolean is_destroyed;
	};

	GAME_EXPORT Game_Object
	game_object_new(Texture texture, glm::vec3 color, glm::vec2 position, glm::vec2 size, glm::vec2 velocity, GLfloat rotation_angle, GLboolean is_solid, GLboolean is_destroyed);

	GAME_EXPORT void
	game_object_free(Game_Object self);

} //namespace end