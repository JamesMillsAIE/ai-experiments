#include <cstdlib>

#include "AiGame.h"

int main()
{
    AiGame* game = new AiGame;

    game->Run("AI Playground", 912, 840, false);

    delete game;

    return EXIT_SUCCESS;
}