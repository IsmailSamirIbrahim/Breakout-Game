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
#define PLAYER_PADDLE_SIZE vec2{100.0, 20.0f}
#define PLAYER_PADDLE_VELOCITY vec2 {500.0f, 0.0f}
#define BALL_RADIUS 12.5f
#define BALL_VELOCITY vec2 {100.0f, -350.0f}

	static Resource_Manager rm = IResource_Manager::get_instance();


	//Helper functions
	inline static void
	_game_ball_update(Game& self, GLfloat delta_time)
	{
		if (!self.ball.ball.is_stuck)
		{
			self.ball.position += self.ball.ball.velocity * delta_time;

			if (self.ball.position.x <= 0.0f)
			{
				self.ball.ball.velocity.x *= -1;
				self.ball.position.x = 0.0f;
			}
			else if (self.ball.position.x + self.ball.size.x >= self.window.width)
			{
				self.ball.ball.velocity.x *= -1;
				self.ball.position.x = self.window.width - self.ball.size.x;
			}
			if (self.ball.position.y <= 0.0f)
			{
				self.ball.ball.velocity.y *= -1;
				self.ball.position.y = 0.0f;
			}
		}
	}

	inline static bool
	_game_collision_check(const Game_Object& obj1, const Game_Object& obj2)
	{
		GLboolean x_collision = (obj1.position.x + obj1.size.x >= obj2.position.x) && (obj2.position.x + obj2.size.x >= obj1.position.x);
		GLboolean y_collision = (obj1.position.y + obj1.size.y >= obj2.position.y) && (obj2.position.y + obj2.size.y >= obj1.position.y);

		return x_collision && y_collision;
	}

	inline static void
	_game_do_collisions(Game& self)
	{
		for (Game_Object& brick : self.levels[self.current_level - 1].bricks)
		{
			if (!brick.brick.is_destroyed)
				if (_game_collision_check(self.ball, brick))
					if (!brick.brick.is_solid)
						brick.brick.is_destroyed = GL_TRUE;
		}
	}
	
	
	// API
	Game
	game_new(GLsizei width, GLsizei height)
	{
		Game self{};

		self.state = Game::STATE_ACTIVE;
		self.current_level = 1;
		self.player = Game_Object{};
		self.ball = Game_Object{};
		self.window = window_new(width, height);
		self.sprite_renderer = nullptr;

		return self;
	}

	void
	game_free(Game self)
	{
		for(auto level : self.levels)
			destruct(level);

		destruct(self.player);
		destruct(self.ball);
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

		image_path = std::string(IMAGE_DIR) + std::string("/images/ball.png");
		resource_manager_load_texture(rm, image_path.c_str(), "ball");

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
		self.player = game_object_player_paddle_new(texture,
			vec2{ self.window.width / 2 - PLAYER_PADDLE_SIZE.x / 2, self.window.height - PLAYER_PADDLE_SIZE.y },
			PLAYER_PADDLE_SIZE,
			PLAYER_PADDLE_VELOCITY
		);

		// ball initialization
		texture = resource_manager_texture(rm, "ball");
		vec2 ball_pos = self.player.position + vec2{ self.player.size.x / 2 - 12.5f, -1 * BALL_RADIUS * 2 };
		self.ball = game_object_ball_new(texture, ball_pos, BALL_RADIUS, BALL_VELOCITY, GL_TRUE);
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

			if (glfwGetKey(self.window.handle, GLFW_KEY_SPACE) == GLFW_PRESS)
				self.keys[GLFW_KEY_SPACE] = GL_TRUE;

			if (glfwGetKey(self.window.handle, GLFW_KEY_SPACE) == GLFW_RELEASE)
				self.keys[GLFW_KEY_SPACE] = GL_FALSE;

			GLfloat velocity = self.player.player_paddle.velocity.x * delta_time;
			if (self.keys[GLFW_KEY_A])
			{
				if (self.player.position.x >= 0)
				{
					self.player.position.x -= velocity;

					if (self.ball.ball.is_stuck)
						self.ball.position.x -= velocity;
				}
			}
			if (self.keys[GLFW_KEY_D])
			{
				if (self.player.position.x <= self.window.width - self.player.size.x)
				{
					self.player.position.x += velocity;
					if (self.ball.ball.is_stuck)
						self.ball.position.x += velocity;
				}

			}
			if (self.keys[GLFW_KEY_SPACE])
			{
				self.ball.ball.is_stuck = GL_FALSE;
			}
		}
	}

	inline static void
	game_update(Game& self, GLfloat delta_time)
	{
		_game_ball_update(self, delta_time);
		_game_do_collisions(self);
	}

	inline static void
	game_render(Game self)
	{
		if (self.state == Game::STATE_ACTIVE)
		{
			// render background
			Texture bkg = resource_manager_texture(rm, "background");
			Game_Object object = game_object_background_new(bkg,
				vec2{ 0.0f, 0.0f},
				vec2{ self.window.width, self.window.height }
			);
			sprite_renderer_render(self.sprite_renderer, object);

			// render level
			for (auto object : self.levels[self.current_level - 1].bricks)
				if (!object.brick.is_destroyed)
					sprite_renderer_render(self.sprite_renderer, object);

			//render player paddle
			sprite_renderer_render(self.sprite_renderer, self.player);

			//render ball
			sprite_renderer_render(self.sprite_renderer, self.ball);
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

			//update
			game_update(self, delta_time);

			// render stuff
			game_render(self);

			// swap buffers
			glfwSwapBuffers(self.window.handle);

			// poll events
			glfwPollEvents();
		}
	}

}