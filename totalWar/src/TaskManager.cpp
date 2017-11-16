#include "TaskManager.h"

TaskManager::TaskManager(UnitMap& units, UnitMap& unitsAi) 
    : 
    m_units(units),
    m_unitsAi(unitsAi),
    m_collisionManager(m_units, m_unitsAi)
{

}

void TaskManager::handleTasks()
{
    for (auto & x : m_units)
    {
        auto unit = x.second.get();
        auto& tasks = unit->getTasks();

        auto verticles = unit->getVerticles();

        for (auto & point : verticles)
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                {
                    Graphics::debugDrawPoint({ point.x + j, point.y + i });
                }
        }

        unit->calculateVerticles();

        if (tasks.size() == 0)
            continue;

        auto task = tasks.front();
       

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

        //Graphics::DebugDrawPoint(unit->getMoveDestination());
    }

    for (auto & x : m_unitsAi)
    {
        auto unit = x.second.get();
        auto& tasks = unit->getTasks();

        auto verticles = unit->getVerticles();

        for (auto & point : verticles)
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                {
                    Graphics::debugDrawPoint({ point.x + j, point.y + i });
                }
        }

        unit->calculateVerticles();

        if (tasks.size() == 0)
            continue;

        auto task = tasks.front();

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

        //Graphics::DebugDrawPoint(unit->getMoveDestination());
    }
}

void TaskManager::Rotate(Unit* unit)
{
    using namespace settings;

    auto unitPos = unit->getPosition();
    auto unitCenter = unitPos;

    unitCenter.x += UNIT_SIZE.x / 2;
    unitCenter.y += UNIT_SIZE.y / 2;

    auto speed = unit->getSpeed();
    auto destination = unit->getMoveDestination();

    destination.x += UNIT_SIZE.x / 2;
    destination.y += UNIT_SIZE.y / 2;

    auto targetAngle = getAngleToPoint(unitCenter, destination);
    if (targetAngle < 0)
        targetAngle = 360.0 + targetAngle;

    if (unit->getTurnedBack())
    {
        if (targetAngle > 0)
            targetAngle -= 180;
        else
            targetAngle += 180;
    }
    auto currentAngle = unit->getAngle();
    auto angleToSet = 0.0;

    auto normalised = std::fmod((targetAngle - currentAngle), 360);
#undef min
    auto absoluteDiffNormalised = std::min(360.0 - normalised, normalised);

    if (std::abs(absoluteDiffNormalised) <= std::abs(1.0))
    {
        auto& tasks = unit->getTasks();
        tasks.pop();
        m_rotationDirections.erase(unit->getId());
        return;
    }
    else if (std::abs(absoluteDiffNormalised) > 1.0 && std::abs(absoluteDiffNormalised) < speed / 60.0)
    {
        angleToSet = targetAngle;
    }
    
    // find if it's closer to rotate clockwise or counter clockwise
    auto rotationDirection = m_rotationDirections.find(unit->getId());

    if (rotationDirection == m_rotationDirections.end())
        if (currentAngle < targetAngle)
        {
            auto clockwiseDistance = targetAngle - currentAngle;
            auto counterClockwiseDistance = currentAngle + (360 - targetAngle);

            if (clockwiseDistance < counterClockwiseDistance)
                m_rotationDirections[unit->getId()] = RotationDirection::CLOCKWISE;
            else
                m_rotationDirections[unit->getId()] = RotationDirection::COUNTER_CLOCKWISE;
        }
        else
        {
            auto clockwiseDistance = targetAngle + (360 - currentAngle);
            auto counterClockwiseDistance = currentAngle - targetAngle;

            if (clockwiseDistance < counterClockwiseDistance)
                m_rotationDirections[unit->getId()] = RotationDirection::CLOCKWISE;
            else
                m_rotationDirections[unit->getId()] = RotationDirection::COUNTER_CLOCKWISE;
        }
    
    if (rotationDirection == m_rotationDirections.end())
        rotationDirection = m_rotationDirections.find(unit->getId());

    if (rotationDirection->second == RotationDirection::CLOCKWISE)
        angleToSet = currentAngle + static_cast<double>(speed) / 60.0;
    else
        angleToSet = currentAngle - static_cast<double>(speed) / 60.0;
            
    if (angleToSet < 0)
        angleToSet = 360 + angleToSet;
    else if (angleToSet > 360)
        angleToSet = angleToSet - 360;

    unit->setAngle(angleToSet);
}

void TaskManager::Flip(Unit* unit)
{
    auto speed = unit->getSpeed();
    auto counter = unit->getMoveCounter();

    if (counter != 25 - speed)
    {
        unit->setMoveCounter(counter + 1);
        return;
    }
    else
        unit->setMoveCounter(0);

    unit->setTurnedBack(!unit->getTurnedBack());
    auto& tasks = unit->getTasks();
    tasks.pop();
}

void TaskManager::Move(Unit* unit)
{
    auto speed = unit->getSpeed();
    auto counter = unit->getMoveCounter();

    if (counter != 25 - speed)
    {
        unit->setMoveCounter(counter + 1);
        return;
    }
    else
        unit->setMoveCounter(0);

    auto position = unit->getPosition();
    auto destination = unit->getMoveDestination();

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
        auto& tasks = unit->getTasks();
        tasks.pop();
        return;
    }

    auto collision = m_collisionManager.checkForCollisions(unit, 10);

    /*if (collision == Collision::FRIENDLY_UNIT)
    {
        auto& tasks = unit->getTasks();
        tasks.pop();
        return;
    }*/

    unit->setPosition(position);

}

void TaskManager::Attack(Unit* unit)
{
    auto target = unit->getAttackTarget();

    if (unit->getToDestroy())
    {
        target->setIsFighting(false);
        target->clearTasks();

        return;
    }
    if (target->getToDestroy())
    {
        unit->setIsFighting(false);
        unit->clearTasks();

        return;
    }

    if (unit->getIsFighting())
    {
        ProcessFighting(unit, target);
        return;
    }

    auto targetPosition = target->getPosition();
    auto position = unit->getPosition();
    auto range = unit->getRange();

    if (m_collisionManager.checkForCollisions(unit, range).getType() != CollisionType::ENEMY_UNIT)
    {
        Move(unit);
    }
    else
    {
        target->setIsFighting(true);
        unit->setIsFighting(true);
    }

    if (unit->getIsFighting() && target->getIsFighting())
        ProcessFighting(unit, target);
}

void TaskManager::ProcessFighting(Unit* unit, Unit* enemyUnit)
{
    using namespace settings;

    if (enemyUnit->getTasks().size() > 0)
        enemyUnit->clearTasks();

    if (unit->getToDestroy())
    {
        enemyUnit->setIsFighting(false);
        enemyUnit->clearTasks();

        return;
    }
    if (enemyUnit->getToDestroy())
    {
        unit->setIsFighting(false);
        unit->clearTasks();

        return;
    }

    //bool attackedFromBehind{ false };
    //bool attackedFromFlank{ false };
    
    // chance of killing enemy soldier every frame = (attack stat/100) * combat speed defined in settings (0.5 by default)
    auto attack = unit->getAttack();
    auto soldiers = unit->getSoldiers();

    if (unit->getSoldiers() > 0 && m_intDistribution(m_mt19937) <= attack * settings::COMBAT_SPEED)
        unit->killSoldiers(1);
    else if (soldiers == 0)
        unit->setToDestroy();

    //auto enemyAttack = enemyUnit->getAttack();
    auto enemySoldiers = enemyUnit->getSoldiers();

    if (enemyUnit->getSoldiers() > 0 && m_intDistribution(m_mt19937) <= attack * COMBAT_SPEED)
        enemyUnit->killSoldiers(1);
    else if (enemySoldiers == 0)
        enemyUnit->setToDestroy();

}