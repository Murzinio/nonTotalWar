#include "..\headers\TaskManager.h"


TaskManager::TaskManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units, SDL_Point unitSize) : m_units(units)
{
    m_unitSize = unitSize;
}

void TaskManager::HandleTasks()
{
    for (auto & x : m_units)
    {
        auto unit = x.second;
        auto& tasks = unit->GetTasks();
        if (tasks.size() == 0)
            continue;

        auto task = tasks.front();
        
        using nonTotalWar::UnitTask;

        switch (task)
        {
            case UnitTask::ROTATE:
                Rotate(unit);
                break;

            case UnitTask::MOVE:
                Move(unit);
                break;

            default:
                break;
        }
    }
}

void TaskManager::Rotate(std::shared_ptr<nonTotalWar::Unit> unit)
{
    auto unitPos = unit->GetPosition();
    auto unitCenter = unitPos;
    
    unitCenter.x += m_unitSize.x / 2;
    unitCenter.y += m_unitSize.y / 2;

    auto speed = unit->GetSpeed();
    auto targetAngle = nonTotalWar::GetAngleToPoint(unitCenter, unit->GetMoveDestination());
    auto currentAngle = unit->GetAngle();
    auto angleToSet = 0.0;

    auto normalised = std::fmod((targetAngle - currentAngle), 360);
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

    destination.x -= m_unitSize.x / 2;
    destination.y -= m_unitSize.y / 2;
        
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

    unit->SetPosition(position);
}