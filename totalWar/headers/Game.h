#pragma once
// SDL
#include <SDL.h>

// game
#include "..\headers\Graphics.h"
#include "..\headers\GameplayManager.h"

namespace nonTotalWar
{
    class Game
    {
    private:
        Graphics m_graphics;
        GameplayManager m_gameplayManager{ m_graphics };
    public:
        Game();
        ~Game();
    };
}
