#include "..\..\headers\units\Unit.h"

using nonTotalWar::Unit;

Unit::Unit(int attack, int defence, int range, int speed, int soldiers, SDL_Point position, double angle)
{
    m_attack = attack;
    m_defence = defence;
    m_range = range;
    m_speed = speed;
    m_position = position;
    m_soldiers = soldiers;
    m_angle = angle;

    CalculateVerticles();
}

Unit::~Unit() {};

SDL_Point Unit::GetPosition()
{
    return m_position;
}

void Unit::SetPosition(SDL_Point newPosition)
{
    m_position = newPosition;
}

SDL_Point Unit::GetMoveDestination()
{
    if (m_tasks.size() > 0)
        return m_moveDestination;

    SDL_Point nullPoint{ 0, 0 };
    return nullPoint;
}

void Unit::CalculateVerticles()
{
    auto unitSize = GetUnitSize();
    auto radians = m_angle * (3.14159265 / 180);

    // TOP LEFT
    double x = unitSize.x / - 2;
    double y = unitSize.y / - 2;

    auto newX = x * std::cos(radians) - y * std::sin(radians);
    auto newY = x * std::sin(radians) + y * std::cos(radians);

    newX = m_position.x + newX - x;
    newY = m_position.y + newY - y;

    m_verticles[0] = { static_cast<int>(newX), static_cast<int>(newY) };

    // TOP RIGHT
    x = unitSize.x / 2;

    newX = x * std::cos(radians) - y * std::sin(radians) + unitSize.x;
    newY = x * std::sin(radians) + y * std::cos(radians);

    newX = m_position.x + newX - x;
    newY = m_position.y + newY - y;

    m_verticles[1] = { static_cast<int>(newX), static_cast<int>(newY) };

    // BOTTOM RIGHT
    x = unitSize.x / 2;
    y = unitSize.y / 2;

    newX = x * std::cos(radians) - y * std::sin(radians) + unitSize.x;
    newY = x * std::sin(radians) + y * std::cos(radians) + unitSize.y;

    newX = m_position.x + newX - x;
    newY = m_position.y + newY - y;

    m_verticles[2] = { static_cast<int>(newX), static_cast<int>(newY) };

    // BOTTOM LEFT
    x = 0;
    y = unitSize.y / -2;

    newX = x * std::cos(radians) - y * std::sin(radians) + unitSize.x / 2;
    newY = x * std::sin(radians) + y * std::cos(radians);

    newX = m_position.x + newX - x;
    newY = m_position.y + newY - y;


    x = unitSize.x / -2;
    y = unitSize.y / 2;

    newX = x * std::cos(radians) - y * std::sin(radians);
    newY = x * std::sin(radians) + y * std::cos(radians) + unitSize.y;

    newX = m_position.x + newX - x;
    newY = m_position.y + newY - y;

    m_verticles[3] = { static_cast<int>(newX), static_cast<int>(newY) };
}

void Unit::SetMoveDestination(SDL_Point destination)
{
    auto unitSize = GetUnitSize();

    m_moveDestination = destination;
    m_moveDestination.x -= unitSize.x / 2;
    m_moveDestination.y -= unitSize.y / 2;
}
