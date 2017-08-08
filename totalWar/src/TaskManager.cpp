#include "..\headers\TaskManager.h"

using nonTotalWar::TaskManager;
using nonTotalWar::settings::UNIT_SIZE;

TaskManager::TaskManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units, std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& unitsAi) : m_units(units), m_unitsAi(unitsAi)
{

}

void TaskManager::HandleTasks()
{
    for (auto & x : m_units)
    {
        auto unit = x.second;
        auto& tasks = unit->GetTasks();

        auto verticles = unit->GetVerticles();

        for (auto & point : verticles)
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                {
                    Graphics::DebugDrawPoint({ point.x + j, point.y + i });
                }
        }

        unit->CalculateVerticles();

        if (tasks.size() == 0)
            continue;

        auto task = tasks.front();
        
        using nonTotalWar::UnitTask;

        switch (task)
        {
            case UnitTask::ROTATE:
                Rotate(unit);
                break;

            case UnitTask::FLIP:
                Flip(unit);
                break;

            case UnitTask::MOVE:
                Move(unit);
                break;

            case UnitTask::ATTACK:
                Attack(unit);
                break;

            default:
                break;
        }

        Graphics::DebugDrawPoint(unit->GetMoveDestination());
    }
}

void TaskManager::Rotate(std::shared_ptr<nonTotalWar::Unit> unit)
{
    auto unitPos = unit->GetPosition();
    auto unitCenter = unitPos;

    unitCenter.x += UNIT_SIZE.x / 2;
    unitCenter.y += UNIT_SIZE.y / 2;

    auto speed = unit->GetSpeed();
    auto destination = unit->GetMoveDestination();

    destination.x += UNIT_SIZE.x / 2;
    destination.y += UNIT_SIZE.y / 2;

    auto targetAngle = nonTotalWar::GetAngleToPoint(unitCenter, destination);

    if (unit->GetTurnedBack())
    {
        if (targetAngle > 0)
            targetAngle -= 180;
        else
            targetAngle += 180;
    }
    auto currentAngle = unit->GetAngle();
    auto angleToSet = 0.0;

    auto normalised = std::fmod((targetAngle - currentAngle), 360);
#undef min
    auto absoluteDiffNormalised = std::min(360.0 - normalised, normalised);

    if (std::abs(absoluteDiffNormalised) <= std::abs(1.0))
    {
        auto& tasks = unit->GetTasks();
        tasks.pop();
        return;
    }
    else if (std::abs(absoluteDiffNormalised) > 1.0 && std::abs(absoluteDiffNormalised) < speed / 60.0)
    {
        angleToSet = targetAngle;
    }
    else if (absoluteDiffNormalised > 0)
        angleToSet = currentAngle + speed / 60.0;
    else if (absoluteDiffNormalised < 0)
        angleToSet = currentAngle - speed / 60.0;

    unit->SetAngle(angleToSet);
}

void TaskManager::Flip(std::shared_ptr<nonTotalWar::Unit> unit)
{
    auto speed = unit->GetSpeed();
    auto counter = unit->GetMoveCounter();

    if (counter != 25 - speed)
    {
        unit->SetMoveCounter(counter + 1);
        return;
    }
    else
        unit->SetMoveCounter(0);

    unit->SetTurnedBack(!unit->GetTurnedBack());
    auto& tasks = unit->GetTasks();
    tasks.pop();
}

void TaskManager::Move(std::shared_ptr<nonTotalWar::Unit> unit)
{
    auto speed = unit->GetSpeed();
    auto counter = unit->GetMoveCounter();

    if (counter != 25 - speed)
    {
        unit->SetMoveCounter(counter + 1);
        return;
    }
    else
        unit->SetMoveCounter(0);

    auto position = unit->GetPosition();
    auto destination = unit->GetMoveDestination();

        
    int valToAddX{ 1 };
    int valToAddY{ 1 };

    if (position.x + 1 == destination.x || position.x - 1 == destination.x)
        valToAddX = 0;
    else if (position.y + 1 == destination.y || position.y - 1 == destination.y)
        valToAddY = 0;

    if (destination.x < position.x)
        valToAddX *= -1;
    if (destination.y < position.y)
        valToAddY *= -1;

    position.x += valToAddX;
    position.y += valToAddY;

    if ((position.x + 1 == destination.x || position.x - 1 == destination.x) 
        && (position.y + 1 == destination.y || position.y - 1 == destination.y))
    {
        auto& tasks = unit->GetTasks();
        tasks.pop();
        return;
    }

    auto collision = CheckForCollisions(unit, 10);

    if (collision == Collision::FRIENDLY_UNIT)
    {
        auto& tasks = unit->GetTasks();
        tasks.pop();
        return;
    }

    unit->SetPosition(position);

}

void TaskManager::Attack(std::shared_ptr<nonTotalWar::Unit> unit)
{
    auto target = unit->GetAttackTarget();
    auto targetPosition = target->GetPosition();
    auto position = unit->GetPosition();
    auto range = unit->GetRange();

    if (CheckForCollisions(unit, range) != Collision::ENEMY_UNIT)
    {
        Move(unit);
    }
    else unit->SetIsFighting(true);

    if (unit->GetIsFighting())
        ProcessFighting(unit, target);
}

void TaskManager::ProcessFighting(std::shared_ptr<nonTotalWar::Unit> unit, std::shared_ptr<nonTotalWar::Unit> enemyUnit)
{
    //TODO
}

SDL_Point TaskManager::GetFuturePosition(std::shared_ptr<nonTotalWar::Unit> unit, int movesForward)
{
    auto position = unit->GetPosition();
    auto destination = unit->GetMoveDestination();

    if (destination.x == 0 && destination.y == 0)
        return position;

    auto futurePosition = position;

    if (destination.x > position.x)
        while (futurePosition.x != position.x + movesForward)
        {
            if(futurePosition.x == destination.x)
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
    nonTotalWar::Graphics::DebugDrawPoint({ futurePosition.x + 1, futurePosition.y + 1 });

    return futurePosition;
}

nonTotalWar::Collision TaskManager::CheckForCollisions(std::shared_ptr<nonTotalWar::Unit> unit, int range)
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