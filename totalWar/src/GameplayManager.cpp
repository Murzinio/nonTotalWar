#include "GameplayManager.h"
#include "unit_test.hpp"

GameplayManager::GameplayManager(Graphics& graphics)
    : m_graphics(graphics), 
    m_taskManager(m_playerUnits, m_aiUnits), 
    m_aiPlayer(m_playerUnits, m_aiUnits)
{
    if (graphics.getTexturesLoaded())
    {
        createUnits();
        m_aiPlayer.createCombatPlan();
        gameLoop();
    }
}

void GameplayManager::createUnits()
{
    //TODO
    for (int i = 1; i < 3; i++)
    {
        Vector2D positionPlayer{ static_cast<float>((i * 180) + 300), static_cast<float>(350) };
        Vector2D positionAi{ static_cast<float>((i * 180) + 300), static_cast<float>(600) };

        m_playerUnits["Hoplites" + std::to_string(i) + "_Player"] = std::make_unique<Hoplites>(i, positionPlayer, 0.0);
        m_aiUnits["Hoplites" + std::to_string(i) + "_AI"] = std::make_unique<Hoplites>(i * -1, positionAi, 180.0);
    }
}

void GameplayManager::gameLoop()
{
    //TODO
    //bool initialPosition = true;
    auto counter = 0;
    SDL_Rect selectionRect{ 0, 0, 0, 0 };

    while (!m_input.getQuitRequested())
    {
        using namespace settings;

        namespace chrono = std::chrono;
        auto now = chrono::high_resolution_clock::now();
        auto lastUpdateInterval = chrono::duration_cast<chrono::milliseconds>(now - m_lastUpdate).count();

        if (lastUpdateInterval < m_updateInterval)
        {
            continue;
        }

        removeDestroyedUnits();

        m_input.handleEvents();
        if (m_input.getKeyUp())
        {
            if (m_input.getKey() == SDLK_b)
            {
                if (m_selectedUnits.size() > 0)
                {
                    Unit* previousUnit = nullptr;

                    for (auto& unit : m_selectedUnits)
                    {
                        if (unit != previousUnit)
                        {
                            previousUnit = unit;
                            unit->addTask(UnitTask::FLIP);
                        }
                    }
                }
            }
        }

        auto selectionRectCreated{ false };

        if (m_input.getMouseLBPressed())
        {
            selectionRect = m_input.getSelectionRectangle();
            selectionRectCreated = true;

            if (selectionRect.w < 0)
            {
                selectionRect.w = std::abs(selectionRect.w);
                selectionRect.x -= selectionRect.w;
            }

            if (selectionRect.h < 0)
            {
                selectionRect.h = std::abs(selectionRect.h);
                selectionRect.y -= selectionRect.h;
            }
        }

        if (m_input.getMouseLBWasReleased())
        {
            auto anyUnitSelected = false;
            for (auto& x : m_playerUnits)
            {
                auto position = x.second->getPosition();
                position.x += UNIT_SIZE.x / 2;
                position.y += UNIT_SIZE.y / 2;

                if ((position.x >= selectionRect.x && position.x <= selectionRect.x + selectionRect.w)
                    & (position.y >= selectionRect.y && position.y <= selectionRect.y + selectionRect.h))
                {
                    x.second->setSelected(true);
                    if (!anyUnitSelected)
                        anyUnitSelected = true;

                    m_selectedUnits.emplace_back(x.second.get());
                }
                else
                {
                    x.second->setSelected(false);
                }
            }

            if (!anyUnitSelected && !isMouseOverFriendlyUnit(m_input.getMousePositionClick()))
            {
                for (auto& x : m_playerUnits)
                {
                    x.second->setSelected(false);
                }

                m_selectedUnits.clear();
            }

            selectionRect = { 0, 0, 0, 0 };
            m_input.clearSelectionRectangle();
        }

        if (m_input.getMouseLBClick())
        {
            if (isMouseOverFriendlyUnit(m_input.getMousePositionClick()))
            {
                m_chosenUnit->setSelected(true);
                m_selectedUnits.clear();

                for (auto& x : m_playerUnits)
                {
                    if (x.second.get() == m_chosenUnit)
                    {
                        m_selectedUnits.emplace_back(x.second.get());
                    }
                }

                m_chosenUnit = nullptr;
            }
            else
            {
                for (auto& x : m_playerUnits)
                {
                    x.second->setSelected(false);
                }
            }   
        }  

        if (m_input.getMouseRBClicked())
        {
            if (IsMouseOverEnemyUnit(m_input.getMousePositionClick()))
            {
                for (auto& unit : m_selectedUnits)
                {
                    unit->clearTasks();
                    unit->addTask(UnitTask::ROTATE);
                    unit->addTask(UnitTask::ATTACK);
                    unit->setMoveDestination(m_input.getMousePositionClick()); //TODO handle required tasks in taskmanager
                    unit->setAttackTarget(m_chosenUnit);
                }

                m_chosenUnit = nullptr;
            }
            else if (!isMouseOverFriendlyUnit(m_input.getMousePositionClick()))
            {
                for (auto& unit : m_selectedUnits)
                {
                    unit->clearTasks();
                    unit->addTask(UnitTask::ROTATE);
                    unit->addTask(UnitTask::MOVE);
                    unit->setMoveDestination(m_input.getMousePositionClick());
                }
            }
        }

        m_taskManager.handleTasks();
        m_lastUpdate = now;

        m_graphics.addToQueue("background\\background", { 0, 0, settings::WINDOW_WIDTH, settings::WINDOW_HEIGHT }, 
            { 0, 0, settings::WINDOW_WIDTH, settings::WINDOW_HEIGHT }, 0, { 0, 0 }, SDL_FLIP_NONE);

        for (auto& x : m_playerUnits)
        {
            auto position = x.second->getPosition();

            SDL_Point center = { UNIT_SIZE.x / 2, UNIT_SIZE.y / 2 };

            position.x += counter * UNIT_SIZE.x / 32;

            SDL_Rect srcRect{ 0, 0, 512, 256 };
            SDL_Rect dstRect{ static_cast<int>(position.x), static_cast<int>(position.y), UNIT_SIZE.x, UNIT_SIZE.y };

            auto flip = SDL_FLIP_NONE;

            if (x.second->getTurnedBack())
                flip = SDL_FLIP_VERTICAL;

            m_graphics.addToQueue("units\\placeholderPlayer", srcRect, dstRect, x.second->getAngle(), center, flip);
            m_graphics.addToQueue("units\\hoplites", srcRect, dstRect, 0.0, center, SDL_FLIP_NONE);

            if (x.second->isSelected())
                m_graphics.addToQueue("units\\placeholderSelected", srcRect, dstRect, x.second->getAngle(), center, SDL_FLIP_NONE);
        }

        for (auto& x : m_aiUnits)
        {
            auto position = x.second->getPosition();
            auto angle = x.second->getAngle();

            SDL_Point center = { UNIT_SIZE.x / 2, UNIT_SIZE.y / 2 };

            position.x += counter * UNIT_SIZE.x / 32;

            SDL_Rect srcRect{ 0, 0, 512, 256 };
            SDL_Rect dstRect{ static_cast<int>(position.x), static_cast<int>(position.y), UNIT_SIZE.x, UNIT_SIZE.y };

            m_graphics.addToQueue("units\\placeholderAI", srcRect, dstRect, angle, center, SDL_FLIP_NONE);
            m_graphics.addToQueue("units\\hoplites", srcRect, dstRect, 0, center, SDL_FLIP_NONE);

            if (x.second->isSelected())
                m_graphics.addToQueue("units\\placeholderSelected", srcRect, dstRect, angle, center, SDL_FLIP_NONE);
        }

        if (selectionRectCreated)
        {
            m_graphics.addSelectionRectToQueue(selectionRect);
        }

        if (m_selectedUnits.size() == 1)
            m_unitStatsBar.draw();

#ifdef _DEBUG
        m_graphics.addTextToQueue({ 0, 0 }, "DEBUG", { 255, 0, 0 }, 14);

#endif // _DEBUG

        m_aiPlayer.updateEnemyPositions();
        m_graphics.renderFrame();

#if UNIT_TEST
            // should only run one iteration when unit testing
            break;
#endif // UNIT_TEST
    }
}

bool GameplayManager::isValidUnitName(const std::string name) const
{
    auto splittedName = splitString(name, '_');

    for (auto& x : m_validUnitNames)
        if (std::find(splittedName.cbegin(), splittedName.cend(), x) == splittedName.cend())
            return false;

    for (auto& x : m_validUnitOwners)
        if (std::find(splittedName.cbegin(), splittedName.cend(), x) == splittedName.cend())
            return false;

    return true;
}

bool GameplayManager::unitExists(const std::string name) const
{
    auto it = m_playerUnits.find(name);
    if (it == m_playerUnits.cend())
        return false;

    it = m_aiUnits.find(name);
    if (it == m_aiUnits.cend())
        return false;

    return true;
}

bool GameplayManager::isMouseOverFriendlyUnit(const SDL_Point mousePosition)
{
    using namespace settings;

    for (auto& unit : m_playerUnits)
    {
        auto position = unit.second->getPosition();
        if (mousePosition.x >= position.x && mousePosition.x <= position.x + UNIT_SIZE.x
            && mousePosition.y >= position.y && mousePosition.y <= position.y + UNIT_SIZE.y)
        {
            m_chosenUnit = unit.second.get();
            return true;
        }
    }

    return false;
}

bool GameplayManager::IsMouseOverEnemyUnit(const SDL_Point mousePosition)
{
    using namespace settings;

    for (auto& unit : m_aiUnits)
    {
        auto position = unit.second->getPosition();
        if (mousePosition.x >= position.x && mousePosition.x <= position.x + UNIT_SIZE.x
            && mousePosition.y >= position.y && mousePosition.y <= position.y + UNIT_SIZE.y)
        {
            m_chosenUnit = unit.second.get();
            return true;
        }
    }

    return false;
}

void GameplayManager::removeDestroyedUnits()
{
    auto it = m_playerUnits.begin();
    while (it != m_playerUnits.end())
    {
        if (it->second->getToDestroy())
        {
            it->second == nullptr;
            it = m_playerUnits.erase(it);
        }
        else
        {
            ++it;
        }
    }

    auto itSelected = m_selectedUnits.begin();
    while (itSelected != m_selectedUnits.end())
    {
        if ((*itSelected)->getToDestroy())
        {
            *itSelected = nullptr;
            itSelected = m_selectedUnits.erase(itSelected);
        }
        else 
        {
            ++itSelected;
        }
    }

    it = m_aiUnits.begin();
    while (it != m_aiUnits.end())
    {
        if (it->second->getToDestroy())
        {
            it->second == nullptr;
            it = m_aiUnits.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
