#include "game/window.h"

#include <stdio.h>
#include <malloc.h>

namespace bko
{
	Window
	window_new(GLsizei width, GLsizei height)
	{
		Window self{};

		self.width = width;
		self.height = height;

		// initialize GLFW and set hints
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		//create window
		self.handle = glfwCreateWindow(width, height, "Breakout", NULL, NULL);
		if (self.handle == NULL)
		{
			printf("Failed to create GLFW window/n");
			glfwTerminate();
			return Window{};
		}
		glfwMakeContextCurrent(self.handle);

		// initialize glew
		glewExperimental = GL_TRUE;
		glewInit();
		glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.


		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return self;
	}

	void
	window_free(Window self)
	{
		glfwTerminate();
	}

}