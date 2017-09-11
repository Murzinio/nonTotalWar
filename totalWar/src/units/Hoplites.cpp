#include "..\..\headers\units\Hoplites.h"

using nonTotalWar::Hoplites;
using nonTotalWar::Vector2D;

Hoplites::Hoplites(int id, Vector2D position, double angle) : Unit(id, 5, 5, 1, 5, 150, position, angle)
{
}

