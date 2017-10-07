#pragma once

#include <queue>
#include <array>
#include <math.h>
#include <memory>

#include "utils.h"
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
        int m_id;

        int m_attack;
        int m_defence;
        int m_range;
        int m_speed;
        int m_soldiers;
        SDL_Point m_unitSize;
        Vector2D m_position;

        double m_angle{ 0.0 };
        bool m_turnedBack{ false };

        bool m_selected{ false };
        bool m_fighting{ false };
        bool m_toDestroy{ false };

        std::queue<UnitTask> m_tasks;
        bool m_taskStarted{ false };
        SDL_Point m_moveDestination;
        std::shared_ptr<Unit> m_attackTarget{ nullptr };
        int m_moveCounter{ 0 };
        std::array<SDL_Point, 4> m_verticles;

    public:
        Unit(int id, int attack, int defence, int range, int speed, int soldiers, Vector2D position, double angle);
        virtual ~Unit();

        Vector2D GetPosition() const;
        void SetPosition(const Vector2D newPosition);
        
        void SetSelected(const bool select) { m_selected = select; }
        bool IsSelected() const { return m_selected; }

        inline void SetAngle(const double angle) { m_angle = angle; }
        inline double GetAngle() const { return m_angle; }

        void AddTask(const UnitTask task)
        {
            m_tasks.push(task);
            m_taskStarted = true;
        }

        std::queue<UnitTask>& GetTasks() { return m_tasks; }

        SDL_Point GetMoveDestination();
        void SetMoveDestination(SDL_Point destination);
        void SetMoveDestination(SDL_Point destination, bool dontResetMoveCounter);
        void SetAttackTarget(std::shared_ptr<Unit> target) { m_attackTarget = target; };
        auto GetAttackTarget() { return m_attackTarget; }
        int GetAttack() { return m_attack; }

        bool GetTaskStarted() { return m_taskStarted; }
        int GetSpeed() { return m_speed; }
        int GetRange() { return m_range; }
        int GetSoldiers() { return m_soldiers; }
        void KillSoldiers(int value) { m_soldiers -= value; }
        void ClearTasks() 
        {
            m_tasks = {};
            m_moveDestination = { 0, 0 };
        }

        void SetMoveCounter(int value) { m_moveCounter = value; }
        int GetMoveCounter() { return m_moveCounter; }

        void CalculateVerticles();
        std::array<SDL_Point, 4> GetVerticles() { return m_verticles; }
        bool GetTurnedBack() { return m_turnedBack; }
        void SetTurnedBack(bool value) { m_turnedBack = value; }

        void SetIsFighting(bool value) { m_fighting = value; }
        bool GetIsFighting() { return m_fighting; }
        void SetToDestroy() { m_toDestroy = true; }
        bool GetToDestroy() { return m_toDestroy; }

        int GetId() { return m_id; }
    };
}
