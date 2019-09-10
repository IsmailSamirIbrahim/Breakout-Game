#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

void processInput(GLFWwindow* window);
void frambuffer_size_callback(GLFWwindow* window, int width, int height);

int
main(int argc, char** argv)
{
	// initialize GLFW and set hints
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, frambuffer_size_callback);

	// initialize GLEW
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::cout << "Error : " << glewGetErrorString(error) << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		//render stuff
		glClearColor(0.0, 0.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//swap buffers
		glfwSwapBuffers(window);
		
		//poll events
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void frambuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}
