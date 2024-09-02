//#include <cstdlib>
//
//#include "AiGame.h"
//
//int main()
//{
//    AiGame* game = new AiGame;
//
//    game->Run("AI Playground", 1088, 768, false);
//
//    delete game;
//
//    return EXIT_SUCCESS;
//}

#include <tmx/tmx.h>

int main()
{
	tmx_map* map = tmx_load("resources/tiled/world_map.tmx");
	if (map == nullptr) 
	{
		tmx_perror("Cannot load map");
		return 1;
	}

	return 0;
}