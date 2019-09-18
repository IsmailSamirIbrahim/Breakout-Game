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
	// API
	Game
	game_new(GLsizei width, GLsizei height)
	{
		Game self{};

		self.state = Game::STATE_ACTIVE;
		self.current_level = 1;
		self.window = window_new(width, height);
		self.rm = IResource_Manager::get_instance();
		self.sprite_renderer = nullptr;

		return self;
	}

	void
	game_free(Game self)
	{
		for(auto level : self.levels)
			destruct(level);

		destruct(self.window);
		destruct(self.rm);
		destruct(self.sprite_renderer);
	}

	void
	game_init(Game& self)
	{
		// load shaders
		string vshader_path = string(SHADER_DIR) + string("/shaders/sprite.vs");
		string fshader_path = string(SHADER_DIR) + string("/shaders/sprite.fs");
		resource_manager_load_program(self.rm, vshader_path.c_str(), fshader_path.c_str(), "sprite");

		// load textures
		string image_path = std::string(IMAGE_DIR) + std::string("/images/awesomeface.png");
		resource_manager_load_texture(self.rm, image_path.c_str(), "face");
		image_path = std::string(IMAGE_DIR) + std::string("/images/container.jpg");
		resource_manager_load_texture(self.rm, image_path.c_str(), "container");

		// load levels
		string level_path = std::string(LEVEL_DIR) + std::string("/levels/standard.lvl");
		Game_Level level_one = game_level_new(self.window.width, self.window.height * 0.5, level_path.c_str());
		self.levels.push_back(level_one);

		// configure shaders
		mat4 projection = ortho(0.0f, static_cast<GLfloat>(self.window.width), static_cast<GLfloat>(self.window.height), 0.0f, -1.0f, 1.0f);
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
		if (glfwGetKey(self.window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(self.window.handle, true);
	}

	inline static void
	game_update(Game self, GLfloat delta_time)
	{

	}

	inline static void
	game_render(Game self)
	{
		if (self.state == Game::STATE_ACTIVE)
		{
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		for (auto object : self.levels[self.current_level - 1].bricks)
			if (!object.is_destroyed)
				sprite_renderer_render(self.sprite_renderer, object);
	}

	void
	game_run(Game self)
	{
		// render loop
		while (!glfwWindowShouldClose(self.window.handle))
		{
			// input
			game_process_input(self, 0);

			// render stuff
			game_render(self);

			// swap buffers
			glfwSwapBuffers(self.window.handle);

			// poll events
			glfwPollEvents();
		}
	}

}