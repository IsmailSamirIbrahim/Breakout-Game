#pragma once

#include "game/Exports.h"
#include "game/sprite_renderer.h"
#include "game/window.h"
#include "game/game_level.h"
#include "component/player_paddle.h"
#include "component/ball.h"

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
		std::vector<Level> levels;
		GLuint current_level;
		Player_Paddle player;
		Ball ball;
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