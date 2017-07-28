#pragma once
#include "unit.h"

namespace nonTotalWar
{
    class Hoplites
        : public Unit
    {
    public:
        Hoplites(SDL_Point position);
        ~Hoplites();
    };

}
