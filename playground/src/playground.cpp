//#include <vld.h>

#include <game/game.h>

using namespace bko;

int
main(int argc, char** argv)
{	
	Game game = game_new(1200, 900);

	game_init(game);

	game_run(game);

	game_free(game);

	return 0;
}

