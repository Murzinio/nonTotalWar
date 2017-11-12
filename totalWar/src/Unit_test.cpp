#include "unit_test.hpp"

#if UNIT_TEST

#include "catch.hpp"

#include "units/Hoplites.h"

TEST_CASE("setup")
{
    Vector2D pos{ 0.f, 0.f };
    Hoplites hoplites(0, pos, 10.0);

    REQUIRE(hoplites.getAngle() == 10.0);
    REQUIRE(hoplites.getAttack() == 5);
    REQUIRE(hoplites.getRange() == 1);
    REQUIRE(hoplites.getSpeed() == 5);
    REQUIRE(hoplites.getSoldiers() == 150);
}

TEST_CASE("tasks")
{
    Vector2D pos{ 0.f, 0.f };
    Hoplites hoplites(0, pos, 0.0);

    hoplites.addTask(UnitTask::ROTATE);

    const auto& tasks = hoplites.getTasks();

    REQUIRE(hoplites.getTaskStarted());
    REQUIRE(tasks.size() == 1);

    hoplites.clearTasks();

    REQUIRE(tasks.size() == 0);
}

#endif