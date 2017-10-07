#include "CollisionManager.h"

using nonTotalWar::CollisionManager;
using nonTotalWar::settings::UNIT_SIZE;

CollisionManager::CollisionManager(const std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units, 
    const std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& unitsAi) : m_units(units), m_unitsAi(unitsAi)
{

}


nonTotalWar::Vector2D CollisionManager::getFuturePosition(const std::shared_ptr<nonTotalWar::Unit> unit, const int movesForward) const
{
    auto position = unit->GetPosition();
    auto destination = unit->GetMoveDestination();

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

    nonTotalWar::Graphics::debugDrawPoint(futurePosition);
    //nonTotalWar::Graphics::DebugDrawPoint({ static_cast<int>(futurePosition.x + 1), static_cast<int>(futurePosition.y + 1) });

    return futurePosition;
}

nonTotalWar::CollisionManager::Collision CollisionManager::checkForCollisions(const std::shared_ptr<nonTotalWar::Unit> unit, const int range) const
{
    if (unit->GetId() < 0)
        return checkForCollisionsPlayer(unit, range);
    else if (unit->GetId() > 0)
        return checkForCollisionsAi(unit, range);
    else
        return Collision(nonTotalWar::CollisionType::NONE, 0, 0);
}

nonTotalWar::CollisionManager::Collision CollisionManager::checkForCollisionsPlayer(const std::shared_ptr<nonTotalWar::Unit> unit, const int range) const
{
    using nonTotalWar::CollisionManager;

    auto collision = Collision(CollisionType::NONE, 0, 0);

    auto moveDestination = unit->GetMoveDestination();

    auto originalPosition = unit->GetPosition();

    unit->SetPosition(getFuturePosition(unit, range));
    unit->CalculateVerticles();

    auto unitVerticles = unit->GetVerticles();

    for (auto & y : m_units)
    {
        if (unit->GetId() == y.second->GetId())
            continue;

        auto otherUnit = y.second;

        auto originalPositionOther = otherUnit->GetPosition();
        otherUnit->SetPosition(getFuturePosition(otherUnit, 10));
        otherUnit->CalculateVerticles();

        auto otherUnitVerticles = otherUnit->GetVerticles();

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
                collision.setId_1(unit->GetId());
                collision.setId_2(otherUnit->GetId());
            }
        }

        otherUnit->SetPosition(originalPositionOther);
        //otherUnit->CalculateVerticles();

    }

    unit->SetPosition(originalPosition);
    //unit->CalculateVerticles();

    return collision;

}

nonTotalWar::CollisionManager::Collision CollisionManager::checkForCollisionsAi(const std::shared_ptr<nonTotalWar::Unit> unit, const int range) const
{
    using nonTotalWar::CollisionManager;

    auto collision = Collision(CollisionType::NONE, 0, 0);

    auto moveDestination = unit->GetMoveDestination();

    auto originalPosition = unit->GetPosition();

    unit->SetPosition(getFuturePosition(unit, range));
    unit->CalculateVerticles();

    auto unitVerticles = unit->GetVerticles();

    for (auto & y : m_unitsAi)
    {
        auto otherUnit = y.second;

        auto originalPositionOther = otherUnit->GetPosition();
        otherUnit->SetPosition(getFuturePosition(otherUnit, range));
        otherUnit->CalculateVerticles();

        auto otherUnitVerticles = otherUnit->GetVerticles();

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
                collision.setId_1(unit->GetId());
                collision.setId_2(otherUnit->GetId());
            }
        }

        otherUnit->SetPosition(originalPositionOther);
        //otherUnit->CalculateVerticles();

    }

    unit->SetPosition(originalPosition);
    //unit->CalculateVerticles();

    return collision;
}