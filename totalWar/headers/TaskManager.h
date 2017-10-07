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
    std::map<std::string, std::shared_ptr<Unit>>& m_units;
    std::map<std::string, std::shared_ptr<Unit>>& m_unitsAi;
    CollisionManager m_collisionManager;
    std::map<int, RotationDirection> m_rotationDirections;

    float m_moveCarryX{ 0.0f };
    float m_moveCarryY{ 0.0f };

    std::random_device rDevice;
    std::mt19937 mt19937{ rDevice() };
    std::uniform_int_distribution<int> intDistribution{ 1, 100 };

public:
    TaskManager(std::map<std::string, std::shared_ptr<Unit>>& units, 
        std::map<std::string, std::shared_ptr<Unit>>& unitsAi);

    void handleTasks();

    void Rotate(const std::shared_ptr<Unit> unit);
    void Flip(const std::shared_ptr<Unit> unit);
    void Move(const std::shared_ptr<Unit> unit);
    void Attack(const std::shared_ptr<Unit> unit);
    void ProcessFighting(const std::shared_ptr<Unit> unit, 
        const std::shared_ptr<Unit> enemyUnit);
};
