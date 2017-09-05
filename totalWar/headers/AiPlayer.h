#pragma once
#include <map>
#include <vector>
#include <memory>
#include "units\Unit.h"

namespace nonTotalWar
{
    enum class AiApproach
    {
        FLANK,
        ENGAGE,
        DEFEND
    };

    class CombatPlan
    {
    private:
        std::vector<std::pair<std::shared_ptr<Unit>,
            std::shared_ptr<Unit>>> engagePair;
    public:
        CombatPlan();
    };

    class AiPlayer
    {
    private:
        const std::map<std::string, std::shared_ptr<Unit>>& m_enemyUnits;
        std::map<std::string, std::shared_ptr<Unit>>& m_units;

        std::shared_ptr<Unit> FindClosestEnemyUnit(const std::shared_ptr<Unit> & unit); //TODO change to finding closest ai unit for each player unit
        
        AiApproach DecideApproach();
        void Flank(); // attack from left and right
        void EngageAllUnits(); // engage closest enemy unit

    public:
        AiPlayer(std::map<std::string, std::shared_ptr<Unit>>& enemyUnits, std::map<std::string, std::shared_ptr<Unit>>& units);
        void CreateCombatPlan();
        void UpdateEnemyPositions();
    };
}
