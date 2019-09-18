#pragma once

#include "game/Exports.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace bko
{
	struct Window
	{
		GLFWwindow* handle;
		GLsizei width;
		GLsizei height;
	};

	GAME_EXPORT Window
	window_new(GLsizei width, GLsizei height);

	GAME_EXPORT void
	window_free(Window self);

	inline static void
	destruct(Window self)
	{
		window_free(self);
	}

} //namespace end