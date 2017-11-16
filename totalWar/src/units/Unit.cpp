#include "units\Unit.h"

Unit::Unit(int id, int attack, int defence, int range, int speed, int soldiers, Vector2D position, double angle)
{
    m_id = id;

    m_attack = attack;
    m_defence = defence;
    m_range = range;
    m_speed = speed;
    m_position = position;
    m_soldiers = soldiers;
    m_angle = angle;

    calculateVerticles();
}

Unit::~Unit() {};

Vector2D Unit::getPosition() const
{
    if (this == nullptr)
        return { 0, 0 };

    return m_position;
}

void Unit::setPosition(Vector2D newPosition)
{
    m_position = newPosition;
}

SDL_Point Unit::getMoveDestination() const
{
    if (m_tasks.size() > 0)
        return m_moveDestination;

    SDL_Point nullPoint{ 0, 0 };
    return nullPoint;
}

void Unit::calculateVerticles()
{
    auto radians = m_angle * (3.14159265 / 180);
    using namespace settings;

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

void Unit::setMoveDestination(SDL_Point destination)
{
    using namespace settings;

    m_moveDestination = destination;
    m_moveDestination.x -= UNIT_SIZE.x / 2;
    m_moveDestination.y -= UNIT_SIZE.y / 2;
    m_moveCounter = 0;
}

void Unit::setMoveDestination(SDL_Point destination, bool dontResetMoveCounter)
{
    using namespace settings;

    m_moveDestination = destination;
    m_moveDestination.x -= UNIT_SIZE.x / 2;
    m_moveDestination.y -= UNIT_SIZE.y / 2;
    if (!dontResetMoveCounter)
        m_moveCounter = 0;
}