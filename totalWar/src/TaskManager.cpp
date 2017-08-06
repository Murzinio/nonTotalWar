#include "..\headers\TaskManager.h"

using nonTotalWar::TaskManager;

TaskManager::TaskManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units) : m_units(units)
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
    
    auto unitSize = GetUnitSize();

    unitCenter.x += unitSize.x / 2;
    unitCenter.y += unitSize.y / 2;

    auto speed = unit->GetSpeed();
    auto destination = unit->GetMoveDestination();

    destination.x += unitSize.x / 2;
    destination.y += unitSize.y / 2;

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

    auto test = CheckForCollisions(unit);

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

    auto unitSize = GetUnitSize();

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

    auto test = CheckForCollisions(unit);

    if (test != Collision::NONE)
    {
        auto& tasks = unit->GetTasks();
        tasks.pop();
        return;
    }

    unit->SetPosition(position);

}

SDL_Point TaskManager::GetFuturePosition(std::shared_ptr<nonTotalWar::Unit> unit, int movesForward)
{
    auto unitSize = nonTotalWar::GetUnitSize();
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

nonTotalWar::Collision TaskManager::CheckForCollisions(std::shared_ptr<nonTotalWar::Unit> unit)
{
    using nonTotalWar::Collision;

    auto collision = Collision::NONE;

    auto moveDestination = unit->GetMoveDestination();
    auto unitSize = GetUnitSize();

    for (auto & x : m_units)
    {
        auto unit = x.second;
        auto originalPosition = unit->GetPosition();
        auto unitVerticles = unit->GetVerticles();

        unit->SetPosition(GetFuturePosition(unit, 10));
        unit->CalculateVerticles();

        for (auto & y : m_units)
        {
            if (x.first == y.first)
                continue;

            auto otherUnit = y.second;
            auto otherUnitVerticles = otherUnit->GetVerticles();

            auto originalPositionOther = otherUnit->GetPosition();
            otherUnit->SetPosition(GetFuturePosition(otherUnit, 10));
            otherUnit->CalculateVerticles();

            auto unitTooFar = true;

            for (auto & v : unitVerticles)
            {
                // if distance to every verticle of other unit is greater than unit rectangle diagonal, skip checks
                for (auto & ov : otherUnitVerticles)
                    if (ov.x != v.x && GetDistanceToPoint(v, ov) <= std::sqrt(unitSize.x*unitSize.x + unitSize.y*unitSize.y))
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

                if (static_cast<int>(areasSum) <= unitSize.x * unitSize.y)
                    collision = Collision::FRIENDLY_UNIT;
            }

            

            otherUnit->SetPosition(originalPositionOther);
            //unit->CalculateVerticles();

        }

        unit->SetPosition(originalPosition);
        //unit->CalculateVerticles();
    }

    return collision;
}