#pragma once

#include "game/Exports.h"
#include "game/sprite_renderer.h"
#include "game/window.h"
#include "game/game_level.h"

#include <vector>

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
		std::vector<Game_Level> levels;
		GLuint current_level;
		Game_Object player;
		Game_Object ball;
		Window window;
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