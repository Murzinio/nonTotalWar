#pragma once
#include <map>
#include <memory>
#include <array>

#include "units\Unit.h"
#include "Input.h"
#include "..\settings.h"
#include "Graphics.h"

namespace nonTotalWar
{
    enum class Collision
    {
        FRIENDLY_UNIT,
        ENEMY_UNIT,
        MAP_BORDER,
        NONE
    };

    class TaskManager
    {
    private:
        std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& m_units;

        float m_moveCarryX{ 0.0f };
        float m_moveCarryY{ 0.0f };
    public:
        TaskManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units);

        void HandleTasks();

        void Rotate(std::shared_ptr<nonTotalWar::Unit> unit);
        void Move(std::shared_ptr<nonTotalWar::Unit> unit);
        Collision CheckForCollisions(std::shared_ptr<nonTotalWar::Unit> unit);
    };
}



