#include "..\..\headers\units\Unit.h"

using nonTotalWar::Unit;

Unit::Unit(size_t attack, size_t defence, size_t range, size_t speed, size_t soldiers, SDL_Point position)
{
    m_attack = attack;
    m_defence = defence;
    m_range = range;
    m_speed = speed;
    m_position = position;
    m_soldiers = soldiers;
}


Unit::~Unit()
{
}

SDL_Point Unit::GetPosition()
{
    return m_position;
}

void Unit::ChangePosition(SDL_Point newPosition)
{
    m_position = newPosition;
}