#include "..\headers\Game.h"

using nonTotalWar::Game;

Game::Game()
{
    SDL_Init(SDL_INIT_VIDEO);
}


Game::~Game()
{
    SDL_Quit();
}
