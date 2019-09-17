#include "game/game.h"
#include "game/game_object.h"

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <assert.h>

using namespace std;
using namespace glm;
using namespace bko;

namespace bko
{
	// Helper Functions
	inline static GLFWwindow*
	glfw_create_window(GLsizei width, GLsizei height)
	{
		// initialize GLFW and set hints
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		//create window
		GLFWwindow* window = glfwCreateWindow(width, height, "Breakout", NULL, NULL);
		if (window == NULL)
		{
			::printf("Failed to create GLFW window/n");
			glfwTerminate();
			return NULL;
		}

		glfwMakeContextCurrent(window);
		
		return window;
	}

	inline static void
	glew_init()
	{
		glewExperimental = GL_TRUE;
		glewInit();
		glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.
	}

	// API
	Game
	game_new(GLsizei width, GLsizei height)
	{
		Game self{};

		self.state = Game::STATE_ACTIVE;
		self.width = width;
		self.height = height;
		self.window = glfw_create_window(width, height);
		self.rm = IResource_Manager::get_instance();
		self.sprite_renderer = nullptr;

		glew_init();

		return self;
	}

	void
	game_free(Game self)
	{
		resource_manager_free(self.rm);
		sprite_renderer_free(self.sprite_renderer);
		glfwTerminate();
	}

	void
	game_init(Game& self)
	{
		// load shaders
		string vshader_path = string(SHADER_DIR) + string("/shaders/sprite.vs");
		string fshader_path = string(SHADER_DIR) + string("/shaders/sprite.fs");
		resource_manager_load_program(self.rm, vshader_path.c_str(), fshader_path.c_str(), "sprite");

		// Load textures
		string image_path = std::string(IMAGE_DIR) + std::string("/images/awesomeface.png");
		resource_manager_load_texture(self.rm, image_path.c_str(), "face");
		image_path = std::string(IMAGE_DIR) + std::string("/images/container.jpg");
		resource_manager_load_texture(self.rm, image_path.c_str(), "container");

		// configure shaders
		mat4 projection = ortho(0.0f, static_cast<GLfloat>(self.width), static_cast<GLfloat>(self.height), 0.0f, -1.0f, 1.0f);
		Program program = resource_manager_program(self.rm, "sprite");
		program_use(program);
		program_int_set(program, "image", 0);
		program_mat4f_set(program, "projection", projection);
		
		// Set render-specific controls
		self.sprite_renderer = sprite_renderer_new(program);
	}

	inline static void
	game_process_input(Game self, GLfloat delta_time)
	{
		if (glfwGetKey(self.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(self.window, true);
	}

	inline static void
	game_update(Game self, GLfloat delta_time)
	{

	}

	inline static void
	game_render(Game self)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Texture texture = resource_manager_texture(self.rm, "face");
		Game_Object object{};
		int k = 50;
		for (int i = 0; i < 50; ++i) {
			if (i * 50 % self.width != 0){
				object = game_object_new(texture, vec3(1.0f, 1.0f, 1.0f), vec2((i * 50) % , 50 + k), vec2(50, 50), vec2{}, 0.0f, GL_FALSE, GL_FALSE);
				k += 50;
			else
				object = game_object_new(texture, vec3(1.0f, 1.0f, 1.0f), vec2((i * 50) % , 50), vec2(50, 50), vec2{}, 0.0f, GL_FALSE, GL_FALSE);

			sprite_renderer_render(self.sprite_renderer, object);
		}
	}

	void
	game_run(Game self)
	{
		// render loop
		while (!glfwWindowShouldClose(self.window))
		{
			// input
			game_process_input(self, 0);

			// render stuff
			game_render(self);

			// swap buffers
			glfwSwapBuffers(self.window);

			// poll events
			glfwPollEvents();
		}
	}

}