#include "units\unit.h"
#include "Graphics.h"
#include "utils.h"

enum class CollisionType
{
    FRIENDLY_UNIT,
    ENEMY_UNIT,
    MAP_BORDER,
    NONE
};

/**
    Calculates/predicts collisions.
*/
class CollisionManager
{
private:
    const UnitMap& m_units;
    const UnitMap& m_unitsAi;

    /**
        Returns the position of unit after number of steps in the future.
    */
    Vector2D getFuturePosition(const Unit* unit, int movesForward) const;

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

    Collision checkForCollisionsPlayer(Unit* unit, const int range) const;
    Collision checkForCollisionsAi(Unit*unit, const int range) const;

public:
    /**
        Takes maps of player and AI units by reference, which are used for collision checks, but not modified.
    */
    CollisionManager(const UnitMap& units, const UnitMap& unitsAi);

    /**
        Checks for collisions using unit passed to it as the center point, range specifies how far it should check for collision, returns Collision with default NONE type
    */
    Collision checkForCollisions(Unit* unit, const int range) const;
};
