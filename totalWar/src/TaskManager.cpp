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
    else if (std::abs(absoluteDiffNormalised) > 1.0 && std::abs(absoluteDiffNormalised) < 10.0)
    {
        angleToSet = targetAngle;
    }
    else if (absoluteDiffNormalised > 0)
        angleToSet = currentAngle + 10.0;
    else if (absoluteDiffNormalised < 0)
        angleToSet = currentAngle - 10.0;

    unit->SetAngle(angleToSet);
}