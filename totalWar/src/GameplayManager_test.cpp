#include "unit_test.hpp"

#if UNIT_TEST

#include "catch.hpp"

#include "GameplayManager.h"
#include "Graphics.h"

TEST_CASE("creating units")
{
    constexpr size_t expectedUnitsCount{ 2 };
    
    Graphics graphics;
    GameplayManager gameplayManager(graphics);

    const auto& playerUnits = gameplayManager.getPlayerUnits();
    const auto& aiUnits = gameplayManager.getAiUnits();

    REQUIRE(playerUnits.size() == expectedUnitsCount);
    REQUIRE(aiUnits.size() == expectedUnitsCount);
}

#endif