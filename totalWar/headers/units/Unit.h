#pragma once

#include <queue>
#include <array>
#include <math.h>
#include <memory>
#include <map>

#include "utils.h"
#include "SDL.h"

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
    Unit* m_attackTarget{ nullptr };
    int m_moveCounter{ 0 };
    std::array<SDL_Point, 4> m_verticles;

public:
    Unit(int id, int attack, int defence, int range, int speed, int soldiers, Vector2D position, double angle);
    virtual ~Unit();

    Vector2D getPosition() const;
    void setPosition(const Vector2D newPosition);
        
    void setSelected(const bool select) { m_selected = select; }
    bool isSelected() const { return m_selected; }

    inline void setAngle(const double angle) { m_angle = angle; }
    inline double getAngle() const { return m_angle; }

    void addTask(const UnitTask task)
    {
        m_tasks.push(task);
        m_taskStarted = true;
    }

    std::queue<UnitTask>& getTasks() { return m_tasks; }

    SDL_Point getMoveDestination() const;
    void setMoveDestination(SDL_Point destination);
    void setMoveDestination(SDL_Point destination, bool dontResetMoveCounter);
    void setAttackTarget(Unit* target) { m_attackTarget = target; };
    Unit* getAttackTarget() const { return m_attackTarget; }
    int getAttack() const { return m_attack; }

    bool getTaskStarted() const { return m_taskStarted; }
    int getSpeed() const { return m_speed; }
    int getRange() const { return m_range; }
    int getSoldiers() const { return m_soldiers; }
    void killSoldiers(int value) { m_soldiers -= value; }
    void clearTasks() 
    {
        m_tasks = {};
        m_moveDestination = { 0, 0 };
    }

    void setMoveCounter(int value) { m_moveCounter = value; }
    int getMoveCounter() const { return m_moveCounter; }

    void calculateVerticles();
    std::array<SDL_Point, 4> getVerticles() { return m_verticles; }
    bool getTurnedBack() const { return m_turnedBack; }
    void setTurnedBack(bool value) { m_turnedBack = value; }

    void setIsFighting(bool value) { m_fighting = value; }
    bool getIsFighting() const { return m_fighting; }
    void setToDestroy() { m_toDestroy = true; }
    bool getToDestroy() const { return m_toDestroy; }

    int getId() const { return m_id; }
};


using UnitMap = std::map<std::string, std::unique_ptr<Unit>>;
using UnitMapRaw = std::map<std::string, Unit*>;