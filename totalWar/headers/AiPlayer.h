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

/**
    Manages AI player: sets targets and tasks for AI units.
*/
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
    /**
        Takes maps of player and AI by reference as parameters, 
        only aiUnits should be managed in this class, player units are used to set tasks for AI.
    */
    AiPlayer(UnitMap& enemyUnits, UnitMap& aiUnits);

    /**
        Decides the strategy for AI units based on number of enemy units.
    */
    void createCombatPlan() const;

    /**
        Get enemy positions again to ensure AI targets are properly set after enemy moves.
    */
    void updateEnemyPositions() const;
};