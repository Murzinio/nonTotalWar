#include "unit_test.hpp"

#if UNIT_TEST

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#endif // UNIT_TEST

#ifdef CATCH_CONFIG_RUNNER

#include <SDL.h>

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    int result = Catch::Session().run(argc, argv);

    SDL_Quit();
    return result;
}
#else
#include "Game.h"

int main(int argc, char* args[])
{
    Game game;
    return 0;
}
#endif // CATCH_CONFIG_RUNNER