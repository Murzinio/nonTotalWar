#pragma once
#include "unit.h"

namespace nonTotalWar
{
    class Hoplites
        : public Unit
    {
    public:
        Hoplites(int id, Vector2D position, double angle);
    };

}
