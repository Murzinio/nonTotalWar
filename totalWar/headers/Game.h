#pragma once
// SDL
#include <SDL.h>

// game
#include "Graphics.h"
#include "GameplayManager.h"

class Game
{
private:
    Graphics m_graphics;
    GameplayManager m_gameplayManager{ m_graphics };
public:
    Game();
    ~Game();
};