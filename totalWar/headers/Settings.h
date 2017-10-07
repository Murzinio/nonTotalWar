#pragma once

namespace settings
{
    constexpr auto FULLSCREEN = false;
    constexpr auto WINDOW_WIDTH = 1600;
    constexpr auto WINDOW_HEIGHT = 900;
    constexpr SDL_Point UNIT_SIZE = { WINDOW_WIDTH / 24, WINDOW_HEIGHT / 24 };
    constexpr auto COMBAT_SPEED = 0.5;
}