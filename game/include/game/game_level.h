#pragma once

#include "game/Exports.h"
#include "game/game_object.h"

#include "GL/glew.h"

#include <vector>

namespace bko
{
	struct Game_Level
	{
		std::vector<Game_Object> bricks;
	};

	GAME_EXPORT Game_Level
	game_level_new(GLuint level_width, GLuint level_height, const char* file_path);

	GAME_EXPORT void
	game_level_free(Game_Level self);

	inline static void
	destruct(Game_Level self)
	{
		game_level_free(self);
	}

	GAME_EXPORT GLboolean
	game_level_is_complete(Game_Level self);

} //namespace end