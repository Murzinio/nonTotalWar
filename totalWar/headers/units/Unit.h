#pragma once

#include "..\utils.h"

namespace nonTotalWar
{
    class Unit
    {
    private:
        size_t m_attack;
        size_t m_defence;
        size_t m_range;
        size_t m_speed;
        size_t m_soldiers;
        
        Vector2D m_position;

    public:
        Unit(size_t attack, size_t defence, size_t range, size_t speed, size_t soldiers, Vector2D position);
        virtual ~Unit();

        nonTotalWar::Vector2D GetPosition();
        void ChangePosition(nonTotalWar::Vector2D newPosition);
    };
}
