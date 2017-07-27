#pragma once
#include "unit.h"

namespace nonTotalWar
{
    class Hoplites
        : public Unit
    {
    public:
        Hoplites(nonTotalWar::Vector2D position);
        ~Hoplites();
    };

}
