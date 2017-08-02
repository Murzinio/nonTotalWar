#pragma once

#include <queue>

#include "..\utils.h"
#include "SDL.h"

namespace nonTotalWar
{
    enum class UnitTask
    {
        ROTATE,
        MOVE,
        ATTACK,
        NONE
    };

    class Unit
    {
    private:
        size_t m_attack;
        size_t m_defence;
        size_t m_range;
        size_t m_speed;
        size_t m_soldiers;

        SDL_Point m_position;

        double m_angle{ 0.0 };

        bool m_selected{ false };

        std::queue<UnitTask> m_tasks;
        bool m_taskStarted{ false };
        SDL_Point m_moveDestination;

    public:
        Unit(size_t attack, size_t defence, size_t range, size_t speed, size_t soldiers, SDL_Point position, double angle);
        virtual ~Unit();

        SDL_Point GetPosition();
        void ChangePosition(SDL_Point newPosition);
        
        void SetSelected(bool select) { m_selected = select; };
        bool IsSelected() { return m_selected; };

        inline void SetAngle(double angle) { m_angle = angle; };
        inline double GetAngle() { return m_angle; };

        void AddTask(UnitTask task)
        {
            m_tasks.push(task);
            m_taskStarted = true;
        }

        std::queue<UnitTask>& GetTasks()
        {
            return m_tasks;
        }

        SDL_Point GetMoveDestination();
        void SetMoveDestination(SDL_Point destination) { m_moveDestination = destination; };

        bool GetTaskStarted() { return m_taskStarted; };
    };
}
