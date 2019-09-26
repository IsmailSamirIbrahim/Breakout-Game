#pragma once

#include <resource_manager/resource_manager.h>

#include <program/program.h>

#include <renderer/window.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <string>
#include <vector>

using namespace std;
using namespace glm;

namespace bko
{
	inline static void
	game_load_shaders()
	{	
		Resource_Manager rm = IResource_Manager::get_instance();
		
		// load shaders
		string vshader_path = string(SHADER_DIR) + string("/shaders/sprite.vs");
		string fshader_path = string(SHADER_DIR) + string("/shaders/sprite.fs");
		resource_manager_load_program(rm, vshader_path.c_str(), fshader_path.c_str(), "sprite");
	}

	inline static void
	game_load_textures()
	{
		Resource_Manager rm = IResource_Manager::get_instance();
		
		// load textures
		string image_path = std::string(IMAGE_DIR) + std::string("/images/background.jpg");
		resource_manager_load_texture(rm, image_path.c_str(), "background");

		image_path = std::string(IMAGE_DIR) + std::string("/images/block.png");
		resource_manager_load_texture(rm, image_path.c_str(), "block");

		image_path = std::string(IMAGE_DIR) + std::string("/images/block_solid.png");
		resource_manager_load_texture(rm, image_path.c_str(), "block_solid");

		image_path = std::string(IMAGE_DIR) + std::string("/images/paddle.png");
		resource_manager_load_texture(rm, image_path.c_str(), "paddle");

		image_path = std::string(IMAGE_DIR) + std::string("/images/ball.png");
		resource_manager_load_texture(rm, image_path.c_str(), "ball");

	}

	inline static void
	game_load_levels(const Window& window, vector<Level>& levels)
	{
		Resource_Manager rm = IResource_Manager::get_instance();

		// load levels
		string level_path = std::string(LEVEL_DIR) + std::string("/levels/standard.lvl");
		Level level = level_new(window.width, window.height * 0.5, level_path.c_str());
		levels.push_back(level);

		level_path = std::string(LEVEL_DIR) + std::string("/levels/few_small_gaps.lvl");
		level = level_new(window.width, window.height * 0.5, level_path.c_str());
		levels.push_back(level);

		level_path = std::string(LEVEL_DIR) + std::string("/levels/space_invader.lvl");
		level = level_new(window.width, window.height * 0.5, level_path.c_str());
		levels.push_back(level);

		level_path = std::string(LEVEL_DIR) + std::string("/levels/bounce_galore.lvl");
		level = level_new(window.width, window.height * 0.5, level_path.c_str());
		levels.push_back(level);
	}
} //namespace end