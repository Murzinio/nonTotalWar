#include "..\headers\GameplayManager.h"

using nonTotalWar::GameplayManager;
using nonTotalWar::Graphics;

GameplayManager::GameplayManager(Graphics& graphics) : m_graphics(graphics), m_taskManager(m_playerUnits)
{
    CreateUnits();
    GameLoop();
}

void GameplayManager::CreateUnits()
{
    //TODO
    for (int i = 1; i < 4; i++)
    {
        SDL_Point positionPlayer{ (int)(i * 150) + 300, 200 };
        SDL_Point positionAi{ (int)(i * 150) + 300, 600 };

        m_playerUnits["Hoplites" + std::to_string(i)] = std::make_unique<Hoplites>(positionPlayer, 0.0);
        m_aiUnits["Hoplites" + std::to_string(i)] = std::make_unique<Hoplites>(positionAi, 0.0);
    }
}

void GameplayManager::GameLoop()
{
    //TODO
    bool initialPosition = true;
    auto counter = 0;
    SDL_Rect selectionRect{ 0, 0, 0, 0 };

    while (!m_input.GetQuitRequested())
    {
        namespace chrono = std::chrono;
        auto now = chrono::high_resolution_clock::now();
        auto lastUpdateInterval = chrono::duration_cast<chrono::milliseconds>(now - m_lastUpdate).count();

        if (lastUpdateInterval < m_updateInterval)
            continue;

        m_input.HandleEvents();

        auto selectionRectCreated{ false };

        if (m_input.GetMouseLBPressed())
        {
            selectionRect = m_input.GetSelectionRectangle();
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

        if (m_input.GetMouseLBWasReleased())
        {
            auto anyUnitSelected{ false };
            for (auto & x : m_playerUnits)
            {
                auto position = x.second->GetPosition();
                auto unitSize = GetUnitSize();
                position.x += unitSize.x / 2;
                position.y += unitSize.y / 2;

                if ((position.x >= selectionRect.x && position.x <= selectionRect.x + selectionRect.w)
                    & (position.y >= selectionRect.y && position.y <= selectionRect.y + selectionRect.h))
                {
                    x.second->SetSelected(!x.second->IsSelected());
                    if (!anyUnitSelected)
                        anyUnitSelected = true;
                }
            }

            if (!anyUnitSelected)
                for (auto & x : m_playerUnits)
                    x.second->SetSelected(false);

            selectionRect = { 0, 0, 0, 0 };
            m_input.ClearSelectionRectangle();
        }

        if (m_input.GetMouseLBClick()) 
            if (IsMouseOverUnit(m_input.GetMousePositionClick()))
            {
                m_chosenUnit->SetSelected(!m_chosenUnit->IsSelected());
                m_chosenUnit = nullptr;
            }
            else
                for (auto & x : m_playerUnits)
                    x.second->SetSelected(false);

        if (m_input.GetMouseRBClicked())
        {
            using nonTotalWar::UnitTask;

            auto selectedUnits = GetSelectedUnits();
            auto task = UnitTask::ROTATE;
            auto task2 = UnitTask::MOVE;

            for (auto & x : selectedUnits)
            {
                x->ClearTasks();
                x->AddTask(task);
                x->AddTask(task2);
                x->SetMoveDestination(m_input.GetMousePositionClick());
            }
        }

        m_taskManager.HandleTasks();

        m_lastUpdate = now;

        m_graphics.AddToQueue("background\\background", { 0, 0, settings::WINDOW_WIDTH, settings::WINDOW_HEIGHT }, { 0, 0, settings::WINDOW_WIDTH, settings::WINDOW_HEIGHT }, 0, { 0, 0 }, SDL_FLIP_NONE);

        for (auto & x : m_playerUnits)
        {
            auto position = x.second->GetPosition();

            auto unitSize = GetUnitSize();
            SDL_Point center = { unitSize.x / 2, unitSize.y / 2 };

            position.x += counter * unitSize.x / 32;

            SDL_Rect srcRect{ 0, 0, 512, 256 };
            SDL_Rect dstRect{ position.x, position.y, unitSize.x, unitSize.y };

            m_graphics.AddToQueue("units\\placeholderPlayer", srcRect, dstRect, x.second->GetAngle(), center, SDL_FLIP_NONE);
            m_graphics.AddToQueue("units\\hoplites", srcRect, dstRect, x.second->GetAngle(), center, SDL_FLIP_NONE);

            if (x.second->IsSelected())
                m_graphics.AddToQueue("units\\placeholderSelected", srcRect, dstRect, x.second->GetAngle(), center, SDL_FLIP_NONE);

        }

        for (auto & x : m_aiUnits)
        {
            auto position = x.second->GetPosition();
            auto angle = x.second->GetAngle();;

            auto unitSize = GetUnitSize();
            SDL_Point center = { unitSize.x / 2, unitSize.y / 2 };

            position.x += counter * unitSize.x / 32;

            SDL_Rect srcRect{ 0, 0, 512, 256 };
            SDL_Rect dstRect{ position.x, position.y, unitSize.x, unitSize.y };

            m_graphics.AddToQueue("units\\placeholderAI", srcRect, dstRect, angle, center, SDL_FLIP_NONE);
            m_graphics.AddToQueue("units\\hoplites", srcRect, dstRect, 0, center, SDL_FLIP_NONE);

            if (x.second->IsSelected())
                m_graphics.AddToQueue("units\\placeholderSelected", srcRect, dstRect, angle, center, SDL_FLIP_NONE);
        }

        if (selectionRectCreated)
        {
            m_graphics.AddSelectionRectToQueue(selectionRect);
        }

        m_graphics.RenderFrame();
    }
    
}

bool GameplayManager::IsValidUnitName(const std::string name)
{
    auto splittedName = nonTotalWar::SplitString(name, '_');

    for (auto & x : m_validUnitNames)
        if (std::find(splittedName.cbegin(), splittedName.cend(), x) == splittedName.cend())
            return false;

    return true;
}

bool GameplayManager::UnitExists(const std::string name)
{
    auto it{ m_playerUnits.find(name) };
    if (it == m_playerUnits.cend())
        return false;

    it = m_aiUnits.find(name);
    if (it == m_aiUnits.cend())
        return false;

    return true;
}

bool GameplayManager::IsMouseOverUnit(SDL_Point mousePosition)
{
    auto unitSize = GetUnitSize();
    for (auto & unit : m_playerUnits)
    {
        auto position = unit.second->GetPosition();
        if (mousePosition.x >= position.x && mousePosition.x <= position.x + unitSize.x
            && mousePosition.y >= position.y && mousePosition.y <= position.y + unitSize.y)
        {
            m_chosenUnit = unit.second;
            return true;
        }
    }

    return false;
}

std::vector<std::shared_ptr<nonTotalWar::Unit>> GameplayManager::GetSelectedUnits()
{
    std::vector<std::shared_ptr<Unit>> units;

    for (auto & x : m_playerUnits)
        if (x.second->IsSelected())
            units.push_back(x.second);

    return units;
}
