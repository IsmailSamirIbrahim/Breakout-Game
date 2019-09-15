#include "game/game.h"

#include <GLFW/glfw3.h>

#include <assert.h>

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

		return self;
	}

	void
	game_free(Game self)
	{

	}

	void
	game_init(Game self)
	{

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

	}
}