#pragma once
#include <map>
#include <memory>
#include <array>

#include "units\Unit.h"
#include "Input.h"
#include "..\settings.h"
#include "Graphics.h"
#include "CollisionManager.h"

namespace nonTotalWar
{
    class TaskManager
    {
    private:
        std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& m_units;
        std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& m_unitsAi;
        CollisionManager m_collisionManager;


        float m_moveCarryX{ 0.0f };
        float m_moveCarryY{ 0.0f };
    public:
        TaskManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units, 
            std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& unitsAi);

        void HandleTasks();

        void Rotate(std::shared_ptr<nonTotalWar::Unit> unit);
        void Flip(std::shared_ptr<nonTotalWar::Unit> unit);
        void Move(std::shared_ptr<nonTotalWar::Unit> unit);
        void Attack(std::shared_ptr<nonTotalWar::Unit> unit);
        void ProcessFighting(std::shared_ptr<nonTotalWar::Unit> unit, 
            std::shared_ptr<nonTotalWar::Unit> enemyUnit);
    };
}



