#pragma once
#include <map>
#include <vector>
#include <memory>
#include "units\Unit.h"

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

    std::shared_ptr<Unit> findClosestEnemyUnit(const std::shared_ptr<Unit>& unit) const; //TODO change to finding closest ai unit for each player unit
        
    AiApproach decideApproach() const;
    void flank() const; // attack from left and right
    void engageAllUnits() const; // engage closest enemy unit

public:
    AiPlayer(std::map<std::string, std::shared_ptr<Unit>>& enemyUnits,
        std::map<std::string, std::shared_ptr<Unit>>& aiUnits);

    void createCombatPlan() const;
    void updateEnemyPositions() const;
};