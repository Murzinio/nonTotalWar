#include "AiPlayer.h"


AiPlayer::AiPlayer(UnitMap& enemyUnits, UnitMap& aiUnits)
    : m_enemyUnits(const_cast<std::map<std::string, std::shared_ptr<Unit>>&>(enemyUnits)),
        m_units(aiUnits)
{
    
}

AiApproach AiPlayer::decideApproach() const
{
    if (m_enemyUnits.size() < m_units.size())
        return AiApproach::FLANK;
    else if (m_enemyUnits.size() == m_units.size())
        return AiApproach::ENGAGE;

        return AiApproach::DEFEND;
}

void AiPlayer::flank() const
{

}

void AiPlayer::engageAllUnits() const
{
    using namespace settings;

    for (const auto & x : m_units)
    {
        auto unit = x.second;
        auto closestEnemy = findClosestEnemyUnit(unit);
        unit->clearTasks();
        unit->addTask(UnitTask::ROTATE);
        unit->addTask(UnitTask::ATTACK);

        auto enemyPosition = closestEnemy->getPosition();
        enemyPosition.x += UNIT_SIZE.x / 2;
        enemyPosition.y += UNIT_SIZE.y / 2;

        unit->setMoveDestination(enemyPosition, true);
        unit->setAttackTarget(closestEnemy);
    }
}

std::shared_ptr<Unit> AiPlayer::findClosestEnemyUnit(const std::shared_ptr<Unit>& unit) const
{
    using namespace settings;

    std::map<std::string, double> distances;

    for (const auto & x : m_enemyUnits)
    {
        auto enemyUnit = x.second;
        if (enemyUnit == nullptr)
            continue;

        auto enemyPosition = enemyUnit->getPosition();
        auto enemyCenter = enemyPosition;
        enemyCenter.x += UNIT_SIZE.x;
        enemyCenter.y += UNIT_SIZE.y;

        if (unit == nullptr)
            continue;

        auto position = unit->getPosition();
        auto center = position;
        center.x += UNIT_SIZE.x;
        center.y += UNIT_SIZE.y;

        distances.emplace(x.first, getDistanceToPoint(enemyCenter, center));
    }

    auto min = WINDOW_WIDTH * 10.0;
    std::string minKey = "";

    // need to search for value by hand
    for (const auto & distance : distances)
    {
        if (distance.second < min)
        {
            min = distance.second;
            minKey = distance.first;
        }
    }

    for (const auto & enemyUnit : m_enemyUnits)
    {
        if (enemyUnit.first == minKey)
            return enemyUnit.second;
    }

    return nullptr;
}

void AiPlayer::createCombatPlan() const
{
    auto approach = decideApproach();

    switch (approach)
    {
    case AiApproach::FLANK:
        flank();
        break;
    case AiApproach::ENGAGE:
        engageAllUnits();
        break;
    default:
        break;
    }
}

void AiPlayer::updateEnemyPositions() const
{
    using namespace settings;

    for (const auto & x : m_units)
    {
        auto unit = x.second;
        auto closestEnemy = findClosestEnemyUnit(unit);
        /*unit->clearTasks();
        unit->addTask(UnitTask::ROTATE);
        unit->addTask(UnitTask::ATTACK);*/
        auto enemyPosition = closestEnemy->getPosition();
        enemyPosition.x += UNIT_SIZE.x / 2;
        enemyPosition.y += UNIT_SIZE.y / 2;

        unit->setMoveDestination(enemyPosition, true);
        unit->setAttackTarget(closestEnemy);
    }
}