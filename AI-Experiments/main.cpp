#include <cstdlib>

#include "AiGame.h"

int main()
{
    AiGame* game = new AiGame;

    game->Run("AI Playground", 1080, 720, false);

    delete game;

    return EXIT_SUCCESS;
}