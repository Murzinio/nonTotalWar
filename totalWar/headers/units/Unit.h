#pragma once

#include <queue>
#include <array>
#include <math.h>
#include <memory>

#include "..\utils.h"
#include "SDL.h"

namespace nonTotalWar
{
    enum class UnitTask
    {
        ROTATE,
        FLIP,
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
        SDL_Point m_unitSize;
        SDL_Point m_position;

        double m_angle{ 0.0 };
        bool m_turnedBack{ false };

        bool m_selected{ false };
        bool m_fighting{ false };

        std::queue<UnitTask> m_tasks;
        bool m_taskStarted{ false };
        SDL_Point m_moveDestination;
        std::shared_ptr<Unit> m_attackTarget{ nullptr };
        int m_moveCounter{ 0 };
        std::array<SDL_Point, 4> m_verticles;

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
        void SetMoveDestination(SDL_Point destination);
        void SetAttackTarget(std::shared_ptr<Unit> target) { m_attackTarget = target; };
        auto GetAttackTarget() { return m_attackTarget; };

        bool GetTaskStarted() { return m_taskStarted; };
        int GetSpeed() { return m_speed; };
        int GetRange() { return m_range; };
        int GetSoldiers() { return m_soldiers; };
        void ClearTasks() 
        {
            m_tasks = {};
            m_moveDestination = { 0, 0 };
        }

        void SetMoveCounter(int value) { m_moveCounter = value; };
        int GetMoveCounter() { return m_moveCounter; };

        void CalculateVerticles();
        std::array<SDL_Point, 4> GetVerticles() { return m_verticles; };
        bool GetTurnedBack() { return m_turnedBack; };
        void SetTurnedBack(bool value) { m_turnedBack = value; };

        void SetIsFighting(bool value) { m_fighting = value; };
        bool GetIsFighting() { return m_fighting; };
    };
}
