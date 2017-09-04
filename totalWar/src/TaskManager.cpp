#include "..\headers\TaskManager.h"

using nonTotalWar::TaskManager;
using nonTotalWar::settings::UNIT_SIZE;
using nonTotalWar::settings::COMBAT_SPEED;

TaskManager::TaskManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units, std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& unitsAi) : m_units(units), m_unitsAi(unitsAi), m_collisionManager(units, unitsAi)
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

    auto collision = m_collisionManager.CheckForCollisions(unit, 10);

    if (collision == nonTotalWar::Collision::FRIENDLY_UNIT)
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
    if (unit->GetIsFighting())
    {
        ProcessFighting(unit, target);
        return;
    }

    auto targetPosition = target->GetPosition();
    auto position = unit->GetPosition();
    auto range = unit->GetRange();

    if (m_collisionManager.CheckForCollisions(unit, range) != Collision::ENEMY_UNIT)
    {
        Move(unit);
    }
    else unit->SetIsFighting(true);

    if (unit->GetIsFighting())
        ProcessFighting(unit, target);
}

void TaskManager::ProcessFighting(std::shared_ptr<nonTotalWar::Unit> unit, std::shared_ptr<nonTotalWar::Unit> enemyUnit)
{
    // chance of killing enemy soldier every frame = (attack stat/100) * combat speed defined in settings (0.5 by default)
    auto attack = unit->GetAttack();
    auto soldiers = unit->GetSoldiers();

    if (unit->GetSoldiers() > 0 && intDistribution(mt19937) <= attack * COMBAT_SPEED)
        unit->KillSoldiers(1);
    else if (soldiers == 0)
        unit->SetToDestroy();

}