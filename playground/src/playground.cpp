//#include <vld.h>

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <program/program.h>
#include <texture/texture.h>
#include <resource_manager/resource_manager.h>

using namespace std;
using namespace bko;

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

	// initialize GLEW and load all OpenGL function pointers
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::cout << "Error : " << glewGetErrorString(error) << std::endl;
		return -1;
	}
	
	// set up vertex data(and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	// create instant of Resource Manager
	Resource_Manager rm = IResource_Manager::get_instance();

	// build and compile our shader program
	std::string vshader_path = std::string(SHADER_DIR) + std::string("/shaders/rectangle.vs");
	std::string fshader_path = std::string(SHADER_DIR) + std::string("/shaders/rectangle.fs");
	resource_manager_load_program(rm, vshader_path.c_str(), fshader_path.c_str(), "rectangle");
	Program program = resource_manager_program(rm, "rectangle");

	resource_manager_load_texture(rm, (std::string(IMAGE_DIR) + std::string("/images/container.jpg")).c_str(), "container");
	resource_manager_load_texture(rm, (std::string(IMAGE_DIR) + std::string("/images/awesomeface.png")).c_str(), "awesomeface");
	Texture texture1 = resource_manager_texture(rm, "container");
	Texture texture2 = resource_manager_texture(rm, "awesomeface");

	program_use(program);
	program_int_set(program, "texture1", 0);
	program_int_set(program, "texture2", 1);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		//render stuff
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind textures on corresponding texture units
		texture_bind(texture1, GL_TEXTURE0);
		texture_bind(texture2, GL_TEXTURE1);

		// draw triangle
		program_use(program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap buffers
		glfwSwapBuffers(window);
		
		// poll events
		glfwPollEvents();
	}
	
	// de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	program_free(program);
	texture_free(texture1);
	texture_free(texture2);
	resource_manager_free(rm);

	// terminate, clearing all previously allocated GLFW resources.
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
