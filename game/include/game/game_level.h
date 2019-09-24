#pragma once

#include "game/Exports.h"
#include "component/brick.h"

#include "GL/glew.h"

#include <vector>

namespace bko
{
	struct Level
	{
		std::vector<Brick> bricks;
	};

	GAME_EXPORT Level
	level_new(GLuint level_width, GLuint level_height, const char* file_path);

	GAME_EXPORT void
	level_free(Level self);

	inline static void
	destruct(Level self)
	{
		level_free(self);
	}

	GAME_EXPORT GLboolean
	level_is_complete(Level self);

} //namespace end