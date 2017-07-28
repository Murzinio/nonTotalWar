#pragma once

#include "..\utils.h"
#include "SDL.h"

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

        SDL_Point m_position;

        bool m_selected{ false };

    public:
        Unit(size_t attack, size_t defence, size_t range, size_t speed, size_t soldiers, SDL_Point position);
        virtual ~Unit();

        SDL_Point GetPosition();
        void ChangePosition(SDL_Point newPosition);
        inline void SetSelected(bool select);
        inline bool IsSelected();
    };

    inline void Unit::SetSelected(bool select)
    {
        m_selected = select;
    }

    inline bool Unit::IsSelected()
    {
        return m_selected;
    }
}
