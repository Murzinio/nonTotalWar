#include "..\headers\TaskManager.h"

using nonTotalWar::TaskManager;
using nonTotalWar::settings::UNIT_SIZE;
using nonTotalWar::settings::COMBAT_SPEED;

TaskManager::TaskManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units, std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& unitsAi) : m_units(units), m_unitsAi(unitsAi), m_collisionManager(units, unitsAi)
{

}

void TaskManager::handleTasks()
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
                    Graphics::debugDrawPoint({ point.x + j, point.y + i });
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

        //Graphics::DebugDrawPoint(unit->GetMoveDestination());
    }

    for (auto & x : m_unitsAi)
    {
        auto unit = x.second;
        auto& tasks = unit->GetTasks();

        auto verticles = unit->GetVerticles();

        for (auto & point : verticles)
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                {
                    Graphics::debugDrawPoint({ point.x + j, point.y + i });
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

        //Graphics::DebugDrawPoint(unit->GetMoveDestination());
    }
}

void TaskManager::Rotate(const std::shared_ptr<nonTotalWar::Unit> unit)
{
    auto unitPos = unit->GetPosition();
    auto unitCenter = unitPos;

    unitCenter.x += UNIT_SIZE.x / 2;
    unitCenter.y += UNIT_SIZE.y / 2;

    auto speed = unit->GetSpeed();
    auto destination = unit->GetMoveDestination();

    destination.x += UNIT_SIZE.x / 2;
    destination.y += UNIT_SIZE.y / 2;

    auto targetAngle = nonTotalWar::getAngleToPoint(unitCenter, destination);
    if (targetAngle < 0)
        targetAngle = 360.0 + targetAngle;

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
        m_rotationDirections.erase(unit->GetId());
        return;
    }
    else if (std::abs(absoluteDiffNormalised) > 1.0 && std::abs(absoluteDiffNormalised) < speed / 60.0)
    {
        angleToSet = targetAngle;
    }
    
    // find if it's closer to rotate clockwise or counter clockwise
    auto rotationDirection = m_rotationDirections.find(unit->GetId());

    if (rotationDirection == m_rotationDirections.end())
        if (currentAngle < targetAngle)
        {
            auto clockwiseDistance = targetAngle - currentAngle;
            auto counterClockwiseDistance = currentAngle + (360 - targetAngle);

            if (clockwiseDistance < counterClockwiseDistance)
                m_rotationDirections[unit->GetId()] = RotationDirection::CLOCKWISE;
            else
                m_rotationDirections[unit->GetId()] = RotationDirection::COUNTER_CLOCKWISE;
        }
        else
        {
            auto clockwiseDistance = targetAngle + (360 - currentAngle);
            auto counterClockwiseDistance = currentAngle - targetAngle;

            if (clockwiseDistance < counterClockwiseDistance)
                m_rotationDirections[unit->GetId()] = RotationDirection::CLOCKWISE;
            else
                m_rotationDirections[unit->GetId()] = RotationDirection::COUNTER_CLOCKWISE;
        }
    
    if (rotationDirection == m_rotationDirections.end())
        rotationDirection = m_rotationDirections.find(unit->GetId());

    if (rotationDirection->second == RotationDirection::CLOCKWISE)
        angleToSet = currentAngle + static_cast<double>(speed) / 60.0;
    else
        angleToSet = currentAngle - static_cast<double>(speed) / 60.0;
            
    if (angleToSet < 0)
        angleToSet = 360 + angleToSet;
    else if (angleToSet > 360)
        angleToSet = angleToSet - 360;

    unit->SetAngle(angleToSet);
}

void TaskManager::Flip(const std::shared_ptr<nonTotalWar::Unit> unit)
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

    auto distance = static_cast<float>(getDistanceToPoint(position, destination));
        
    float valToAddX{ (destination.x - position.x) / distance };
    float valToAddY{ (destination.y - position.y) / distance };

    if (std::abs(position.x - destination.x) < 2.0)
        valToAddX = 0;
    else if (std::abs(position.y - destination.y) < 2.0)
        valToAddY = 0;

    position.x += valToAddX;
    position.y += valToAddY;

    if (std::abs(position.x - destination.x) < 2.0
        && std::abs(position.y - destination.y) < 2.0)
    {
        auto& tasks = unit->GetTasks();
        tasks.pop();
        return;
    }

    auto collision = m_collisionManager.checkForCollisions(unit, 10);

    /*if (collision == nonTotalWar::Collision::FRIENDLY_UNIT)
    {
        auto& tasks = unit->GetTasks();
        tasks.pop();
        return;
    }*/

    unit->SetPosition(position);

}

void TaskManager::Attack(std::shared_ptr<nonTotalWar::Unit> unit)
{
    auto target = unit->GetAttackTarget();

    if (unit->GetToDestroy())
    {
        target->SetIsFighting(false);
        target->ClearTasks();

        return;
    }
    if (target->GetToDestroy())
    {
        unit->SetIsFighting(false);
        unit->ClearTasks();

        return;
    }

    if (unit->GetIsFighting())
    {
        ProcessFighting(unit, target);
        return;
    }

    auto targetPosition = target->GetPosition();
    auto position = unit->GetPosition();
    auto range = unit->GetRange();

    if (m_collisionManager.checkForCollisions(unit, range).getType() != CollisionType::ENEMY_UNIT)
    {
        Move(unit);
    }
    else
    {
        target->SetIsFighting(true);
        unit->SetIsFighting(true);
    }

    if (unit->GetIsFighting() && target->GetIsFighting())
        ProcessFighting(unit, target);
}

void TaskManager::ProcessFighting(std::shared_ptr<nonTotalWar::Unit> unit, std::shared_ptr<nonTotalWar::Unit> enemyUnit)
{
    if (enemyUnit->GetTasks().size() > 0)
        enemyUnit->ClearTasks();

    if (unit->GetToDestroy())
    {
        enemyUnit->SetIsFighting(false);
        enemyUnit->ClearTasks();

        return;
    }
    if (enemyUnit->GetToDestroy())
    {
        unit->SetIsFighting(false);
        unit->ClearTasks();

        return;
    }

    bool attackedFromBehind{ false };
    bool attackedFromFlank{ false };
    
    // chance of killing enemy soldier every frame = (attack stat/100) * combat speed defined in settings (0.5 by default)
    auto attack = unit->GetAttack();
    auto soldiers = unit->GetSoldiers();

    if (unit->GetSoldiers() > 0 && intDistribution(mt19937) <= attack * COMBAT_SPEED)
        unit->KillSoldiers(1);
    else if (soldiers == 0)
        unit->SetToDestroy();

    auto enemyAttack = enemyUnit->GetAttack();
    auto enemySoldiers = enemyUnit->GetSoldiers();

    if (enemyUnit->GetSoldiers() > 0 && intDistribution(mt19937) <= attack * COMBAT_SPEED)
        enemyUnit->KillSoldiers(1);
    else if (enemySoldiers == 0)
        enemyUnit->SetToDestroy();

}