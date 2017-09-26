#include "units\unit.h"
#include "Graphics.h"
#include "utils.h"

namespace nonTotalWar
{
    enum class CollisionType
    {
        FRIENDLY_UNIT,
        ENEMY_UNIT,
        MAP_BORDER,
        NONE
    };

    class CollisionManager
    {
    private:
        const std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& m_units;
        const std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& m_unitsAi;

        Vector2D getFuturePosition(std::shared_ptr<nonTotalWar::Unit> unit, int movesForward) const;

        class Collision
        {
        private:
            CollisionType m_type{ CollisionType::NONE };

            int m_unitId_1{ 0 };
            int m_unitId_2{ 0 };

        public:

            Collision(CollisionType type, int id_1, int id_2)
            {
                m_type = type;
                m_unitId_1 = id_1;
                m_unitId_2 = id_2;
            }

            void setType(const CollisionType type) { m_type = type; }
            void setId_1(const int id) { m_unitId_1 = id; }
            void setId_2(const int id) { m_unitId_2 = id; }

            CollisionType getType() const { return m_type; }
            int getId_1() const { return  m_unitId_1; }
            int getId_2() const { return  m_unitId_2; }
        };

        Collision checkForCollisionsPlayer(const std::shared_ptr<nonTotalWar::Unit> unit, const int range) const;
        Collision checkForCollisionsAi(const std::shared_ptr<nonTotalWar::Unit> unit, const int range) const;

    public:
        CollisionManager(const std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units, 
            const std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& unitsAi);

        Collision checkForCollisions(const std::shared_ptr<nonTotalWar::Unit> unit, const int range) const;
        
    };
}

