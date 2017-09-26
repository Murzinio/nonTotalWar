#pragma once
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <array>
#include <chrono>

#include <SDL.h>

// game
#include "units\Hoplites.h"
#include "Graphics.h"
#include "Input.h"
#include "TaskManager.h"
#include "UnitStatsBar.h"
#include "AiPlayer.h"

namespace nonTotalWar
{
    class GameplayManager
    {
    private:
        Graphics& m_graphics;
        UnitStatsBar m_unitStatsBar{ m_graphics, m_selectedUnits };
        Input m_input;

        std::map<std::string, std::shared_ptr<Unit>> m_playerUnits; 
        std::map<std::string, std::shared_ptr<Unit>> m_aiUnits;
        std::map<std::string, std::shared_ptr<Unit>> m_selectedUnits;
        //TODO unit counts by type in std::map

        AiPlayer m_aiPlayer; //TODO HumanPlayer class

        TaskManager m_taskManager;

        void createUnits();
        void gameLoop();
        bool isValidUnitName(const std::string name) const;
        bool unitExists(const std::string name) const;

        const std::array<const std::string, 1> m_validUnitNames
        { "Hoplites" };
        const std::array<const std::string, 2> m_validUnitOwners
        { "Player", "AI" };
        // unit name should be constructed from m_validUnitName, number and owner (i. e. "Hoplites_1_AI")

        std::chrono::high_resolution_clock::time_point m_lastUpdate = std::chrono::high_resolution_clock::now();
        static constexpr float m_updateInterval{ 1000.0f / 1200.0f };

        std::shared_ptr<Unit> m_chosenUnit;
        bool isMouseOverFriendlyUnit(const SDL_Point mousePosition);
        bool IsMouseOverEnemyUnit(const SDL_Point mousePosition);

        void removeDestroyedUnits();

    public:
        GameplayManager(Graphics& graphics);
    };
}
