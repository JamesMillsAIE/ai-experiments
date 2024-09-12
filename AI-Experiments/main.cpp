#ifdef _DEBUG
#include <crtdbg.h>
#endif // _DEBUG

#include <cstdlib>

#include "AiGame.h"

int main()
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

    AiGame* game = new AiGame;

    game->Run("AI Playground", 912, 840, false);

    delete game;

    return EXIT_SUCCESS;
}