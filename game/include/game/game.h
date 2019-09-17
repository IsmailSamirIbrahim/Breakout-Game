#pragma once

#include "game/Exports.h"
#include "game/sprite_renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
		GLsizei width;
		GLsizei height;
		GLFWwindow* window;
		Resource_Manager rm;
		Sprite_Renderer sprite_renderer;
	};

	GAME_EXPORT Game
	game_new(GLsizei width, GLsizei height);

	GAME_EXPORT void
	game_free(Game self);

	GAME_EXPORT void
	game_init(Game& self);

	GAME_EXPORT void
	game_run(Game self);

} //namespace end