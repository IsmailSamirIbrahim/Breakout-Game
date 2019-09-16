#pragma once

#include <game/Exports.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <program/program.h>
#include <texture/texture.h>

namespace bko
{
	struct Sprite_Renderer
	{
		Program program;
		Texture texture;
		glm::vec2 size;
		glm::vec2 position;
		GLfloat rotate_angle;
		glm::vec3 color;
		GLuint quad_vao;
	};

	GAME_EXPORT Sprite_Renderer
	sprite_renderer_new(const Program& program, const Texture& texture, const glm::vec2& size, const glm::vec2& position, GLfloat rotate_angle, const glm::vec3& color);

	GAME_EXPORT void
	sprite_renderer_free(Sprite_Renderer self);

	GAME_EXPORT void
	sprite_renderer_render(Sprite_Renderer self);

} //namespace end