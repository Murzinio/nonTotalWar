#include "units\unit.h"
#include "Graphics.h"
#include "utils.h"

namespace nonTotalWar
{
    enum class Collision
    {
        FRIENDLY_UNIT,
        ENEMY_UNIT,
        MAP_BORDER,
        NONE
    };

    class CollisionManager
    {
    private:
        std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& m_units;
        std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& m_unitsAi;

        SDL_Point GetFuturePosition(std::shared_ptr<nonTotalWar::Unit> unit, int movesForward);

    public:
        CollisionManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units, std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& unitsAi);

        Collision CheckForCollisions(std::shared_ptr<nonTotalWar::Unit> unit, int range);
    };
}

