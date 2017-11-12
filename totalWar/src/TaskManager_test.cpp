#include "unit_test.hpp"

#if UNIT_TEST

#include "catch.hpp"

#include "TaskManager.h"
#include "units/Hoplites.h"

TEST_CASE("rotation")
{
    UnitMap playerUnits;
    UnitMap aiUnits;
    Vector2D pos{ 0.f, 0.f };

    std::string name{ "Hoplites1_Player" };

    playerUnits.insert({ name, std::make_shared<Hoplites>(0, pos, 0) });

    TaskManager manager(playerUnits, aiUnits);

    playerUnits.at(name)->setMoveDestination({ 256, 256 });
    playerUnits.at(name)->addTask(UnitTask::ROTATE);
    playerUnits.at(name)->addTask(UnitTask::MOVE);

    manager.handleTasks();
    REQUIRE(playerUnits.at(name)->getAngle() != 0.0);
}

TEST_CASE("moving")
{
    UnitMap playerUnits;
    UnitMap aiUnits;
    Vector2D pos{ 0.f, 0.f };

    std::string name{ "Hoplites1_Player" };

    playerUnits.insert({ name, std::make_shared<Hoplites>(0, pos, 0) });
    const auto& unit = playerUnits.at(name);

    TaskManager manager(playerUnits, aiUnits);

    unit->setMoveDestination({ 256, 256 });
    unit->addTask(UnitTask::MOVE);
    unit->setMoveCounter(25 - unit->getSpeed());
    
    manager.handleTasks();
    REQUIRE(playerUnits.at(name)->getPosition().x != pos.x);
}

#endif