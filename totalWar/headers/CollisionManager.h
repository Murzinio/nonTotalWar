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
        std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& m_units;
        std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& m_unitsAi;

        Vector2D GetFuturePosition(std::shared_ptr<nonTotalWar::Unit> unit, int movesForward);
        

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

            void SetType(CollisionType type) { m_type = type; };
            void SetId_1(int id) { m_unitId_1 = id; };
            void SetId_2(int id) { m_unitId_2 = id; };

            CollisionType GetType() { return m_type; };
            int GetId_1() { return  m_unitId_1; };
            int GetId_2() { return  m_unitId_2; };
        };

        Collision CheckForCollisionsPlayer(std::shared_ptr<nonTotalWar::Unit> unit, int range);
        Collision CheckForCollisionsAi(std::shared_ptr<nonTotalWar::Unit> unit, int range);

    public:
        CollisionManager(std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& units, std::map<std::string, std::shared_ptr<nonTotalWar::Unit>>& unitsAi);

        Collision CheckForCollisions(std::shared_ptr<nonTotalWar::Unit> unit, int range);
        
    };
}

