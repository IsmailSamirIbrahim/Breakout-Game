#include "game/game.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <assert.h>

using namespace std;
using namespace glm;
using namespace bko;

namespace bko
{
	Game
	game_new(GLuint width, GLuint height)
	{
		Game self{};

		self.state = Game::STATE_ACTIVE;
		self.width = width;
		self.height = height;
		self.rm = IResource_Manager::get_instance();

		return self;
	}

	void
	game_free(Game self)
	{

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
		Texture texture = resource_manager_texture(self.rm, "face");

		// configure shaders
		mat4 projection = ortho(0.0f, static_cast<GLfloat>(self.width), static_cast<GLfloat>(self.height), 0.0f, -1.0f, 1.0f);
		Program program = resource_manager_program(self.rm, "sprite");
		program_use(program);
		program_int_set(program, "image", 0);
		program_mat4f_set(program, "projection", projection);
		
		// Set render-specific controls
		self.sprite_renderer = sprite_renderer_new(program, texture, vec2(200, 200), vec2(300, 400), 45.0f, vec3(0.0f, 1.0f, 0.0f));
	}

	void
	game_process_input(Game self, GLfloat delta_time)
	{

	}

	void
	game_update(Game self, GLfloat delta_time)
	{

	}

	void
	game_render(Game self)
	{
		sprite_renderer_render(self.sprite_renderer);
	}
}