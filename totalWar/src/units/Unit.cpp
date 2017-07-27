#include "..\..\headers\units\Unit.h"

using nonTotalWar::Unit;
using nonTotalWar::Vector2D;

Unit::Unit(size_t attack, size_t defence, size_t range, size_t speed, size_t soldiers, Vector2D position)
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

Vector2D Unit::GetPosition()
{
    return m_position;
}

void Unit::ChangePosition(Vector2D newPosition)
{
    m_position = newPosition;
}