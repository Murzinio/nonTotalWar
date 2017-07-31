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

namespace nonTotalWar
{
    class GameplayManager
    {
    private:
        Graphics& m_graphics;
        Input m_input;

        std::map<std::string, std::shared_ptr<Unit>> m_playerUnits;
        std::map<std::string, std::shared_ptr<Unit>> m_aiUnits;
        std::map<std::string, std::shared_ptr<Unit>> m_selectedUnits;
        //TODO unit counts by type in std::map

        TaskManager m_taskManager;

        void CreateUnits();
        void GameLoop();
        bool IsValidUnitName(const std::string name);
        bool UnitExists(const std::string name);

        const std::array<const std::string, 1> m_validUnitNames
        { "Hoplites" };
        // unit name should be constructed from m_validUnitName and a number (i. e. "Hoplites_1")

        std::chrono::high_resolution_clock::time_point m_lastUpdate = std::chrono::high_resolution_clock::now();
        static constexpr float m_updateInterval{ 1000.0f / 60.0f };

        std::shared_ptr<Unit> m_chosenUnit;
        bool IsMouseOverUnit(SDL_Point mousePosition);
        std::vector<std::shared_ptr<Unit>> GetSelectedUnits();

    public:
        GameplayManager(Graphics& graphics);
        ~GameplayManager();
    };
}
