#include "..\..\headers\units\Unit.h"

using nonTotalWar::Unit;
using nonTotalWar::Vector2D;
using nonTotalWar::settings::UNIT_SIZE;

Unit::Unit(int attack, int defence, int range, int speed, int soldiers, nonTotalWar::Vector2D position, double angle)
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

Vector2D Unit::GetPosition()
{
    return m_position;
}

void Unit::SetPosition(Vector2D newPosition)
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
    auto radians = m_angle * (3.14159265 / 180);

    // TOP LEFT
    double x = UNIT_SIZE.x / - 2;
    double y = UNIT_SIZE.y / - 2;

    auto newX = x * std::cos(radians) - y * std::sin(radians);
    auto newY = x * std::sin(radians) + y * std::cos(radians);

    newX = m_position.x + newX - x;
    newY = m_position.y + newY - y;

    m_verticles[0] = { static_cast<int>(newX), static_cast<int>(newY) };

    // TOP RIGHT
    x = UNIT_SIZE.x / 2;

    newX = x * std::cos(radians) - y * std::sin(radians) + UNIT_SIZE.x;
    newY = x * std::sin(radians) + y * std::cos(radians);

    newX = m_position.x + newX - x;
    newY = m_position.y + newY - y;

    m_verticles[1] = { static_cast<int>(newX), static_cast<int>(newY) };

    // BOTTOM RIGHT
    x = UNIT_SIZE.x / 2;
    y = UNIT_SIZE.y / 2;

    newX = x * std::cos(radians) - y * std::sin(radians) + UNIT_SIZE.x;
    newY = x * std::sin(radians) + y * std::cos(radians) + UNIT_SIZE.y;

    newX = m_position.x + newX - x;
    newY = m_position.y + newY - y;

    m_verticles[2] = { static_cast<int>(newX), static_cast<int>(newY) };

    // BOTTOM LEFT
    x = 0;
    y = UNIT_SIZE.y / -2;

    newX = x * std::cos(radians) - y * std::sin(radians) + UNIT_SIZE.x / 2;
    newY = x * std::sin(radians) + y * std::cos(radians);

    newX = m_position.x + newX - x;
    newY = m_position.y + newY - y;


    x = UNIT_SIZE.x / -2;
    y = UNIT_SIZE.y / 2;

    newX = x * std::cos(radians) - y * std::sin(radians);
    newY = x * std::sin(radians) + y * std::cos(radians) + UNIT_SIZE.y;

    newX = m_position.x + newX - x;
    newY = m_position.y + newY - y;

    m_verticles[3] = { static_cast<int>(newX), static_cast<int>(newY) };
}

void Unit::SetMoveDestination(SDL_Point destination)
{
    m_moveDestination = destination;
    m_moveDestination.x -= UNIT_SIZE.x / 2;
    m_moveDestination.y -= UNIT_SIZE.y / 2;
    m_moveCounter = 0;
}

void Unit::SetMoveDestination(SDL_Point destination, bool dontResetMoveCounter)
{
    m_moveDestination = destination;
    m_moveDestination.x -= UNIT_SIZE.x / 2;
    m_moveDestination.y -= UNIT_SIZE.y / 2;
    if (!dontResetMoveCounter)
        m_moveCounter = 0;
}