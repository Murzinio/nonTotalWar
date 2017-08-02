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
        int m_attack;
        int m_defence;
        int m_range;
        int m_speed;
        int m_soldiers;

        SDL_Point m_position;

        double m_angle{ 0.0 };

        bool m_selected{ false };

        std::queue<UnitTask> m_tasks;
        bool m_taskStarted{ false };
        SDL_Point m_moveDestination;
        int m_moveCounter{ 0 };

    public:
        Unit(int attack, int defence, int range, int speed, int soldiers, SDL_Point position, double angle);
        virtual ~Unit();

        SDL_Point GetPosition();
        void SetPosition(SDL_Point newPosition);
        
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
        int GetSpeed() { return m_speed; };
        void ClearTasks() 
        {
            m_tasks = {};
            m_moveDestination = { 0, 0 };
        }

        void SetMoveCounter(int value) { m_moveCounter = value; };
        int GetMoveCounter() { return m_moveCounter; };
    };
}
