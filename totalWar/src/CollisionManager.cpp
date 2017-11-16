#include "CollisionManager.h"

CollisionManager::CollisionManager(const UnitMap& units, const UnitMap& unitsAi)
    : 
    m_units(units), 
    m_unitsAi(unitsAi)
{

}


Vector2D CollisionManager::getFuturePosition(const Unit* unit, const int movesForward) const
{
    auto position = unit->getPosition();
    auto destination = unit->getMoveDestination();

    if (destination.x == 0 && destination.y == 0)
        return position;

    auto futurePosition = position;

    if (destination.x > position.x)
        while (futurePosition.x != position.x + movesForward)
        {
            if (futurePosition.x == destination.x)
                break;

            ++futurePosition.x;
        }
    else
        while (futurePosition.x != position.x - movesForward)
        {
            if (futurePosition.x == destination.x)
                break;

            --futurePosition.x;
        }

    if (destination.y > position.y)
        while (futurePosition.y != position.y + movesForward)
        {
            if (futurePosition.y == destination.y)
                break;

            ++futurePosition.y;
        }
    else
        while (futurePosition.y != position.y - movesForward)
        {
            if (futurePosition.y == destination.y)
                break;

            --futurePosition.y;
        }

    Graphics::debugDrawPoint(futurePosition);
    //Graphics::DebugDrawPoint({ static_cast<int>(futurePosition.x + 1), static_cast<int>(futurePosition.y + 1) });

    return futurePosition;
}

CollisionManager::Collision CollisionManager::checkForCollisions(Unit* unit, const int range) const
{
    if (unit->getId() < 0)
        return checkForCollisionsPlayer(unit, range);
    else if (unit->getId() > 0)
        return checkForCollisionsAi(unit, range);
    else
        return Collision(CollisionType::NONE, 0, 0);
}

CollisionManager::Collision CollisionManager::checkForCollisionsPlayer(Unit* unit, const int range) const
{
    using namespace settings;

    auto collision = Collision(CollisionType::NONE, 0, 0);

    auto moveDestination = unit->getMoveDestination();
    auto originalPosition = unit->getPosition();

    unit->setPosition(getFuturePosition(unit, range));
    unit->calculateVerticles();

    auto unitVerticles = unit->getVerticles();

    for (auto& y : m_units)
    {
        if (unit->getId() == y.second->getId())
            continue;

        auto otherUnit = y.second.get();

        auto originalPositionOther = otherUnit->getPosition();
        otherUnit->setPosition(getFuturePosition(otherUnit, 10));
        otherUnit->calculateVerticles();

        auto otherUnitVerticles = otherUnit->getVerticles();

        auto unitTooFar = true;

        for (auto & v : unitVerticles)
        {
            // if distance to every verticle of other unit is greater than unit rectangle diagonal, skip checks
            for (auto & ov : otherUnitVerticles)
                if (ov.x != v.x && getDistanceToPoint(v, ov) <= std::sqrt(UNIT_SIZE.x * UNIT_SIZE.x + UNIT_SIZE.y * UNIT_SIZE.y))
                    unitTooFar = false;

            if (unitTooFar)
                continue;

            // calculate area of triangle built on first unit verticle and pair of other unit verticles, 
            // if the sum of 4 areas is greater than the area of rectangle, the first unit verticle is not inside other unit
            // to do, skip if distance between verticles is greater than unit rectangle diagonal
            auto areasSum = 0.0;

            for (int i = 0; i < 4; ++i)
            {
                auto a = 0.0;
                auto b = 0.0;
                auto c = 0.0;

                if (i < 3)
                {
                    a = getDistanceToPoint(v, otherUnitVerticles[i]);
                    b = getDistanceToPoint(v, otherUnitVerticles[i + 1]);
                    c = getDistanceToPoint(otherUnitVerticles[i], otherUnitVerticles[i + 1]);
                }
                else
                {
                    a = getDistanceToPoint(v, otherUnitVerticles[i]);
                    b = getDistanceToPoint(v, otherUnitVerticles[0]);
                    c = getDistanceToPoint(otherUnitVerticles[i], otherUnitVerticles[0]);
                }

                areasSum += getTriangleArea(a, b, c);
            }

            if (static_cast<int>(areasSum) <= UNIT_SIZE.x * UNIT_SIZE.y)
            {
                collision.setType(CollisionType::ENEMY_UNIT);
                collision.setId_1(unit->getId());
                collision.setId_2(otherUnit->getId());
            }
        }

        otherUnit->setPosition(originalPositionOther);
        //otherUnit->calculateVerticles();

    }

    unit->setPosition(originalPosition);
    //unit->calculateVerticles();

    return collision;

}

CollisionManager::Collision CollisionManager::checkForCollisionsAi(Unit* unit, const int range) const
{
    using namespace settings;

    auto collision = Collision(CollisionType::NONE, 0, 0);

    auto moveDestination = unit->getMoveDestination();

    auto originalPosition = unit->getPosition();

    unit->setPosition(getFuturePosition(unit, range));
    unit->calculateVerticles();

    auto unitVerticles = unit->getVerticles();

    for (auto& y : m_unitsAi)
    {
        auto otherUnit = y.second.get();

        auto originalPositionOther = otherUnit->getPosition();
        otherUnit->setPosition(getFuturePosition(otherUnit, range));
        otherUnit->calculateVerticles();

        auto otherUnitVerticles = otherUnit->getVerticles();

        auto unitTooFar = true;

        for (auto & v : unitVerticles)
        {
            // if distance to every verticle of other unit is greater than unit rectangle diagonal, skip checks
            for (auto & ov : otherUnitVerticles)
                if (ov.x != v.x && getDistanceToPoint(v, ov) <= std::sqrt(UNIT_SIZE.x*UNIT_SIZE.x + UNIT_SIZE.y*UNIT_SIZE.y))
                    unitTooFar = false;

            if (unitTooFar)
                continue;

            // calculate area of triangle built on first unit verticle and pair of other unit verticles, 
            // if the sum of 4 areas is greater than the area of rectangle, the first unit verticle is not inside other unit
            // to do, skip if distance between verticles is greater than unit rectangle diagonal
            auto areasSum = 0.0;

            for (int i = 0; i < 4; ++i)
            {
                auto a = 0.0;
                auto b = 0.0;
                auto c = 0.0;

                if (i < 3)
                {
                    a = getDistanceToPoint(v, otherUnitVerticles[i]);
                    b = getDistanceToPoint(v, otherUnitVerticles[i + 1]);
                    c = getDistanceToPoint(otherUnitVerticles[i], otherUnitVerticles[i + 1]);
                }
                else
                {
                    a = getDistanceToPoint(v, otherUnitVerticles[i]);
                    b = getDistanceToPoint(v, otherUnitVerticles[0]);
                    c = getDistanceToPoint(otherUnitVerticles[i], otherUnitVerticles[0]);
                }

                areasSum += getTriangleArea(a, b, c);
            }

            if (static_cast<int>(areasSum) <= UNIT_SIZE.x * UNIT_SIZE.y)
            {
                collision.setType(CollisionType::FRIENDLY_UNIT);
                collision.setId_1(unit->getId());
                collision.setId_2(otherUnit->getId());
            }
        }

        otherUnit->setPosition(originalPositionOther);
        //otherUnit->calculateVerticles();

    }

    unit->setPosition(originalPosition);
    //unit->calculateVerticles();

    return collision;
}