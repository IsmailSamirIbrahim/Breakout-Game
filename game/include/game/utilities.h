#pragma once

#include <resource_manager/resource_manager.h>

#include <program/program.h>

#include <Texture/texture.h>

#include <renderer/window.h>

#include <component/brick.h>

#include <component/player_paddle.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <string>
#include <vector>

using namespace std;
using namespace glm;

namespace bko
{
#define PLAYER_PADDLE_SIZE vec2{100.0, 20.0f}
#define PLAYER_PADDLE_VELOCITY vec2 {500.0f, 0.0f}
#define BALL_RADIUS 12.5f
#define BALL_VELOCITY vec2 {100.0f, -350.0f}

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

	inline static void
	game_reset_level(vector<Level>& levels, GLuint current_level)
	{
		for (Brick& brick : levels[current_level - 1].bricks)
			brick.is_destroyed = GL_FALSE;
	}

	inline static void
	game_reset_player(Player_Paddle& player, const Window& window)
	{
		// player reseting
		player.sprite.position = vec2{ window.width / 2 - PLAYER_PADDLE_SIZE.x / 2, window.height - PLAYER_PADDLE_SIZE.y };
		player.velocity = PLAYER_PADDLE_VELOCITY;
	}

	inline static void
	game_reset_ball(Ball& ball, const Player_Paddle& player)
	{
		// ball reseting
		vec2 ball_pos = player.sprite.position + vec2{ player.sprite.size.x / 2 - BALL_RADIUS, -1 * BALL_RADIUS * 2 };
		ball.sprite.position = ball_pos;
		ball.velocity = BALL_VELOCITY;
		ball.is_stuck = GL_TRUE;
	}
} //namespace end