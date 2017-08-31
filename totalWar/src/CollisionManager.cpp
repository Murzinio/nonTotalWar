#include "..\headers\CollisionManager.h"

using nonTotalWar::CollisionManager;
using nonTotalWar::settings::UNIT_SIZE;

CollisionManager::CollisionManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units, std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& unitsAi) : m_units(units), m_unitsAi(unitsAi)
{

}

nonTotalWar::Vector2D CollisionManager::GetFuturePosition(std::shared_ptr<nonTotalWar::Unit> unit, int movesForward)
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

    nonTotalWar::Graphics::DebugDrawPoint(futurePosition);
    nonTotalWar::Graphics::DebugDrawPoint({ static_cast<int>(futurePosition.x + 1), static_cast<int>(futurePosition.y + 1) });

    return futurePosition;
}

nonTotalWar::Collision CollisionManager::CheckForCollisions(std::shared_ptr<nonTotalWar::Unit> unit, int range)
{
    using nonTotalWar::Collision;

    auto collision = Collision::NONE;

    auto moveDestination = unit->GetMoveDestination();

    for (auto & x : m_units)
    {
        auto unit = x.second;
        auto originalPosition = unit->GetPosition();

        unit->SetPosition(GetFuturePosition(unit, range));
        unit->CalculateVerticles();

        auto unitVerticles = unit->GetVerticles();

        for (auto & y : m_units)
        {
            if (x.first == y.first)
                continue;

            auto otherUnit = y.second;

            auto originalPositionOther = otherUnit->GetPosition();
            otherUnit->SetPosition(GetFuturePosition(otherUnit, 10));
            otherUnit->CalculateVerticles();

            auto otherUnitVerticles = otherUnit->GetVerticles();

            auto unitTooFar = true;

            for (auto & v : unitVerticles)
            {
                // if distance to every verticle of other unit is greater than unit rectangle diagonal, skip checks
                for (auto & ov : otherUnitVerticles)
                    if (ov.x != v.x && GetDistanceToPoint(v, ov) <= std::sqrt(UNIT_SIZE.x * UNIT_SIZE.x + UNIT_SIZE.y * UNIT_SIZE.y))
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
                        a = GetDistanceToPoint(v, otherUnitVerticles[i]);
                        b = GetDistanceToPoint(v, otherUnitVerticles[i + 1]);
                        c = GetDistanceToPoint(otherUnitVerticles[i], otherUnitVerticles[i + 1]);
                    }
                    else
                    {
                        a = GetDistanceToPoint(v, otherUnitVerticles[i]);
                        b = GetDistanceToPoint(v, otherUnitVerticles[0]);
                        c = GetDistanceToPoint(otherUnitVerticles[i], otherUnitVerticles[0]);
                    }

                    areasSum += GetTriangleArea(a, b, c);
                    std::cout << a << std::endl << b << std::endl << c << std::endl;
                }

                if (static_cast<int>(areasSum) <= UNIT_SIZE.x * UNIT_SIZE.y)
                    collision = Collision::FRIENDLY_UNIT;
            }

            otherUnit->SetPosition(originalPositionOther);
            //unit->CalculateVerticles();

        }

        unit->SetPosition(originalPosition);
        //unit->CalculateVerticles();
    }

    for (auto & x : m_units)
    {
        auto unit = x.second;
        auto originalPosition = unit->GetPosition();

        unit->SetPosition(GetFuturePosition(unit, range));
        unit->CalculateVerticles();

        auto unitVerticles = unit->GetVerticles();

        for (auto & y : m_unitsAi)
        {

            auto otherUnit = y.second;

            auto originalPositionOther = otherUnit->GetPosition();
            otherUnit->SetPosition(GetFuturePosition(otherUnit, 10));
            otherUnit->CalculateVerticles();

            auto otherUnitVerticles = otherUnit->GetVerticles();

            auto unitTooFar = true;

            for (auto & v : unitVerticles)
            {
                // if distance to every verticle of other unit is greater than unit rectangle diagonal, skip checks
                for (auto & ov : otherUnitVerticles)
                    if (ov.x != v.x && GetDistanceToPoint(v, ov) <= std::sqrt(UNIT_SIZE.x*UNIT_SIZE.x + UNIT_SIZE.y*UNIT_SIZE.y))
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
                        a = GetDistanceToPoint(v, otherUnitVerticles[i]);
                        b = GetDistanceToPoint(v, otherUnitVerticles[i + 1]);
                        c = GetDistanceToPoint(otherUnitVerticles[i], otherUnitVerticles[i + 1]);
                    }
                    else
                    {
                        a = GetDistanceToPoint(v, otherUnitVerticles[i]);
                        b = GetDistanceToPoint(v, otherUnitVerticles[0]);
                        c = GetDistanceToPoint(otherUnitVerticles[i], otherUnitVerticles[0]);
                    }

                    areasSum += GetTriangleArea(a, b, c);
                    std::cout << a << std::endl << b << std::endl << c << std::endl;
                }

                if (static_cast<int>(areasSum) <= UNIT_SIZE.x * UNIT_SIZE.y)
                    collision = Collision::ENEMY_UNIT;
            }

            otherUnit->SetPosition(originalPositionOther);
            //unit->CalculateVerticles();

        }

        unit->SetPosition(originalPosition);
        //unit->CalculateVerticles();
    }

    return collision;
}