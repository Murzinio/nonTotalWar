#include "Game.h"

Game::Game()
{
    SDL_Init(SDL_INIT_VIDEO);
}


Game::~Game()
{
    SDL_Quit();
}
