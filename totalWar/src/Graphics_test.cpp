#include "unit_test.hpp"

#if UNIT_TEST

#include "catch.hpp"

#define private public
#include "Graphics.h"
#undef private

TEST_CASE("textures are loaded")
{
    Graphics graphics;
    REQUIRE(graphics.m_texturesLoaded);
}

TEST_CASE("queue items added properly")
{
    Graphics graphics;

    graphics.addToQueue(
        "unit_test",
        { 0, 0 },
        { 0, 0 },
        0.0,
        { 0, 0 },
        SDL_FLIP_NONE
    );
    REQUIRE(graphics.m_renderQueue.size() == 1);

    graphics.addToQueue(
        "fake",
        { 0, 0 },
        { 0, 0 },
        0.0,
        { 0, 0 },
        SDL_FLIP_NONE
    );
    REQUIRE(graphics.m_renderQueue.size() == 1);

    graphics.renderFrame();
    REQUIRE(graphics.m_renderQueue.size() == 0);
}

#endif