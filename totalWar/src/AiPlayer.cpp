#include "..\headers\AiPlayer.h"

using nonTotalWar::AiPlayer;
using nonTotalWar::AiApproach;
using nonTotalWar::Unit;

AiPlayer::AiPlayer(std::map<std::string, std::shared_ptr<Unit>>& enemyUnits, std::map<std::string, std::shared_ptr<Unit>>& units) : m_enemyUnits(const_cast<std::map<std::string, std::shared_ptr<Unit>>&>(enemyUnits)),
    m_units(units)
{
    
}

AiApproach AiPlayer::DecideApproach()
{
    if (m_enemyUnits.size() < m_units.size())
        return AiApproach::FLANK;
    else if (m_enemyUnits.size() == m_units.size())
        return AiApproach::ENGAGE;

        return AiApproach::DEFEND;
}

void AiPlayer::Flank()
{

}

void AiPlayer::EngageAllUnits()
{
    for (const auto & x : m_units)
    {
        auto unit = x.second;
        auto closestEnemy = FindClosestEnemyUnit(unit);
        unit->ClearTasks();
        unit->AddTask(UnitTask::ROTATE);
        unit->AddTask(UnitTask::ATTACK);
        unit->SetMoveDestination(closestEnemy->GetPosition());
        unit->SetAttackTarget(closestEnemy);
    }
}

std::shared_ptr<Unit> AiPlayer::FindClosestEnemyUnit(const std::shared_ptr<Unit>& unit)
{
    using namespace nonTotalWar::settings;

    std::map<std::string, double> distances;

    for (const auto & x : m_enemyUnits)
    {
        auto enemyUnit = x.second;
        if (enemyUnit == nullptr)
            continue;

        auto enemyPosition = enemyUnit->GetPosition();
        auto enemyCenter = enemyPosition;
        enemyCenter.x += UNIT_SIZE.x;
        enemyCenter.y += UNIT_SIZE.y;

        if (unit == nullptr)
            continue;

        auto position = unit->GetPosition();
        auto center = position;
        center.x += UNIT_SIZE.x;
        center.y += UNIT_SIZE.y;

        distances.emplace(x.first, GetDistanceToPoint(enemyCenter, center));
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

void AiPlayer::CreateCombatPlan()
{
    auto approach = DecideApproach();

    switch (approach)
    {
    case nonTotalWar::AiApproach::FLANK:
        Flank();
        break;
    case nonTotalWar::AiApproach::ENGAGE:
        EngageAllUnits();
        break;
    default:
        break;
    }
}

void AiPlayer::UpdateEnemyPositions()
{
    for (const auto & x : m_units)
    {
        auto unit = x.second;
        auto closestEnemy = FindClosestEnemyUnit(unit);
        /*unit->ClearTasks();
        unit->AddTask(UnitTask::ROTATE);
        unit->AddTask(UnitTask::ATTACK);*/
        unit->SetMoveDestination(closestEnemy->GetPosition(), true);
        unit->SetAttackTarget(closestEnemy);
    }
}