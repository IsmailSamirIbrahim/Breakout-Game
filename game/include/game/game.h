#pragma once

#include "game/Exports.h"
#include "game/sprite_renderer.h"

#include <GL/glew.h>

#include <resource_manager/resource_manager.h>

namespace bko
{
	struct Game
	{
		enum STATE
		{
			STATE_ACTIVE,
			STATE_MENU,
			STATE_WIN
		};

		STATE state;
		GLboolean keys[1024];
		GLuint width;
		GLuint height;
		Resource_Manager rm;
		Sprite_Renderer sprite_renderer;

	};

	GAME_EXPORT Game
	game_new(GLuint width, GLuint height);

	GAME_EXPORT void
	game_free(Game self);

	GAME_EXPORT void
	game_init(Game& self);

	GAME_EXPORT void
	game_process_input(Game self, GLfloat delta_time);

	GAME_EXPORT void
	game_update(Game self, GLfloat delta_time);

	GAME_EXPORT void
	game_render(Game self);

} //namespace end