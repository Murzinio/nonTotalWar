#include "..\..\headers\units\Unit.h"

using nonTotalWar::Unit;

Unit::Unit(size_t attack, size_t defence, size_t range, size_t speed, size_t soldiers, SDL_Point position, double angle)
{
    m_attack = attack;
    m_defence = defence;
    m_range = range;
    m_speed = speed;
    m_position = position;
    m_soldiers = soldiers;
    m_angle = angle;
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