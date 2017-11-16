#pragma once
#include <map>
#include <memory>
#include <array>
#include <random>

#include "units\Unit.h"
#include "Input.h"
#include "Settings.h"
#include "Graphics.h"
#include "CollisionManager.h"

enum class RotationDirection
{
    CLOCKWISE,
    COUNTER_CLOCKWISE
};

class TaskManager
{
private:
    UnitMap& m_units;
    UnitMap& m_unitsAi;
    CollisionManager m_collisionManager;
    std::map<int, RotationDirection> m_rotationDirections;

    float m_moveCarryX{ 0.0f };
    float m_moveCarryY{ 0.0f };

    std::random_device m_rDevice;
    std::mt19937 m_mt19937{ m_rDevice() };
    std::uniform_int_distribution<int> m_intDistribution{ 1, 100 };

public:
    TaskManager(UnitMap& units, UnitMap& unitsAi);

    void handleTasks();

    void Rotate(Unit* unit);
    void Flip(Unit* unit);
    void Move(Unit* unit);
    void Attack(Unit* unit);
    void ProcessFighting(Unit* unit,
        Unit* enemyUnit);
};
