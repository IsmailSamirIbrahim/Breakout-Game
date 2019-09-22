#include "game/game.h"
#include "game/game_object.h"

#include <glm/gtc/matrix_transform.hpp>

#include <resource_manager/resource_manager.h>

#include <string>
#include <assert.h>

using namespace std;
using namespace glm;
using namespace bko;

namespace bko
{
	static Resource_Manager rm = IResource_Manager::get_instance();

	// API
	Game
	game_new(GLsizei width, GLsizei height)
	{
		Game self{};

		self.state = Game::STATE_ACTIVE;
		self.current_level = 4;
		self.player = Game_Object{};
		self.window = window_new(width, height);
		self.sprite_renderer = nullptr;

		return self;
	}

	void
	game_free(Game self)
	{
		for(auto level : self.levels)
			destruct(level);

		destruct(self.window);
		destruct(self.sprite_renderer);
	}

	void
	game_init(Game& self)
	{
		// load shaders
		string vshader_path = string(SHADER_DIR) + string("/shaders/sprite.vs");
		string fshader_path = string(SHADER_DIR) + string("/shaders/sprite.fs");
		resource_manager_load_program(rm, vshader_path.c_str(), fshader_path.c_str(), "sprite");

		// load textures
		string image_path = std::string(IMAGE_DIR) + std::string("/images/background.jpg");
		resource_manager_load_texture(rm, image_path.c_str(), "background");

		image_path = std::string(IMAGE_DIR) + std::string("/images/block.png");
		resource_manager_load_texture(rm, image_path.c_str(), "block");

		image_path = std::string(IMAGE_DIR) + std::string("/images/block_solid.png");
		resource_manager_load_texture(rm, image_path.c_str(), "block_solid");

		image_path = std::string(IMAGE_DIR) + std::string("/images/paddle.png");
		resource_manager_load_texture(rm, image_path.c_str(), "paddle");

		// load levels
		string level_path = std::string(LEVEL_DIR) + std::string("/levels/standard.lvl");
		Game_Level level = game_level_new(self.window.width, self.window.height * 0.5, level_path.c_str());
		self.levels.push_back(level);

		level_path = std::string(LEVEL_DIR) + std::string("/levels/few_small_gaps.lvl");
		level = game_level_new(self.window.width, self.window.height * 0.5, level_path.c_str());
		self.levels.push_back(level);

		level_path = std::string(LEVEL_DIR) + std::string("/levels/space_invader.lvl");
		level = game_level_new(self.window.width, self.window.height * 0.5, level_path.c_str());
		self.levels.push_back(level);

		level_path = std::string(LEVEL_DIR) + std::string("/levels/bounce_galore.lvl");
		level = game_level_new(self.window.width, self.window.height * 0.5, level_path.c_str());
		self.levels.push_back(level);

		// configure shaders
		mat4 projection = ortho(0.0f, static_cast<GLfloat>(self.window.width), static_cast<GLfloat>(self.window.height), 0.0f, -1.0f, 1.0f);
		Program program = resource_manager_program(rm, "sprite");
		program_use(program);
		program_int_set(program, "image", 0);
		program_mat4f_set(program, "projection", projection);
		
		// Set render-specific controls
		self.sprite_renderer = sprite_renderer_new(program);

		// player initialization
		Texture texture = resource_manager_texture(rm, "paddle");
		self.player = game_object_new(texture,
			vec3{1.0f, 1.0f, 1.0f},
			vec2{ self.window.width / 2 - 100 / 2, self.window.height - 20 },
			vec2{ 100.0, 20.0f },
			vec2{ 500.0f, 0.0f },
			0.0f,
			GL_FALSE,
			GL_FALSE);
	}

	inline static void
	game_process_input(Game& self, GLfloat delta_time)
	{
		if (self.state == Game::STATE_ACTIVE)
		{
			if (glfwGetKey(self.window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(self.window.handle, true);

			if (glfwGetKey(self.window.handle, GLFW_KEY_A) == GLFW_PRESS)
				self.keys[GLFW_KEY_A] = GL_TRUE;

			if (glfwGetKey(self.window.handle, GLFW_KEY_A) == GLFW_RELEASE)
				self.keys[GLFW_KEY_A] = GL_FALSE;

			if (glfwGetKey(self.window.handle, GLFW_KEY_D) == GLFW_PRESS)
				self.keys[GLFW_KEY_D] = GL_TRUE;

			if (glfwGetKey(self.window.handle, GLFW_KEY_D) == GLFW_RELEASE)
				self.keys[GLFW_KEY_D] = GL_FALSE;

			GLfloat velocity = self.player.velocity.x * delta_time;
			if (self.keys[GLFW_KEY_A])
			{
				if (self.player.position.x >= 0)
					self.player.position.x -= velocity;
			}
			if (self.keys[GLFW_KEY_D])
			{
				if (self.player.position.x <= self.window.width - self.player.size.x)
					self.player.position.x += velocity;
			}
		}
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
			// render background
			Texture bkg = resource_manager_texture(rm, "background");
			Game_Object object = game_object_new(bkg,
				vec3{ 1.0f, 1.0f, 1.0f },
				vec2{ 0.0f, 0.0f},
				vec2{ self.window.width, self.window.height },
				vec2{},
				0.0f,
				GL_FALSE,
				GL_FALSE);
			sprite_renderer_render(self.sprite_renderer, object);

			// render level
			for (auto object : self.levels[self.current_level - 1].bricks)
				if (!object.is_destroyed)
					sprite_renderer_render(self.sprite_renderer, object);

			//render player paddle
			sprite_renderer_render(self.sprite_renderer, self.player);
		}
	}

	void
	game_run(Game self)
	{
		float current_frame;
		float delta_time = 0.0f;
		float last_frame = 0.0f;
		// render loop
		while (!glfwWindowShouldClose(self.window.handle))
		{
			current_frame = glfwGetTime();
			delta_time = current_frame - last_frame;
			last_frame = current_frame;
			// input
			game_process_input(self, delta_time);

			// render stuff
			game_render(self);

			// swap buffers
			glfwSwapBuffers(self.window.handle);

			// poll events
			glfwPollEvents();
		}
	}

}