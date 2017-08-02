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
    auto position = unit->GetPosition();
    auto destination = unit->GetMoveDestination();
    destination.x -= m_unitSize.x / 2;
    destination.y -= m_unitSize.y/ 2;

    auto vectorLength = std::sqrt(std::pow(destination.x, 2.0) + std::pow(destination.y, 2.0));
    auto speed = unit->GetSpeed();
        
    auto valToAddX = static_cast<float>(nonTotalWar::settings::WINDOW_WIDTH) / 100000.0f * static_cast<float>(speed);
    auto valToAddY = static_cast<float>(nonTotalWar::settings::WINDOW_HEIGHT) / 100000.0f * static_cast<float>(speed);

    if (destination.x < position.x)
        valToAddX *= -1;
    if (destination.y < position.y)
        valToAddY *= -1;

    if (std::abs(valToAddX) < 1.0)
        m_moveCarryX += valToAddX;
    else
        position.x += static_cast<int>(valToAddX);

    if (std::abs(valToAddY) < 1.0)
        m_moveCarryY += valToAddY;
    else
        position.y += static_cast<int>(valToAddY);

    if (position.x + valToAddX > destination.x && destination.x > position.x || position.y + valToAddY > destination.y && destination.y > position.y)
    {
        auto& tasks = unit->GetTasks();
        tasks.pop();
        return;
    }
    else if (position.x + valToAddX < destination.x && destination.x < position.x || position.y + valToAddY < destination.y && destination.y < position.y)
    {
        auto& tasks = unit->GetTasks();
        tasks.pop();
        return;
    }

    if (std::abs(m_moveCarryX) > 1.0)
    {
        position.x += static_cast<int>(m_moveCarryX);
        m_moveCarryX = 0.0f;
    }

    if (std::abs(m_moveCarryY) > 1.0)
    {
        position.y += static_cast<int>(m_moveCarryY);
        m_moveCarryY = 0.0f;
    }

    unit->SetPosition(position);
}