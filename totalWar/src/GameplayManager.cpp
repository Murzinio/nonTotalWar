#include "..\headers\GameplayManager.h"

using nonTotalWar::GameplayManager;
using nonTotalWar::Graphics;

GameplayManager::GameplayManager(Graphics& graphics) : m_graphics(graphics), m_taskManager(m_playerUnits, m_graphics.GetUnitSize())
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

    while (!m_input.QuitRequested())
    {

        namespace chrono = std::chrono;
        auto now = chrono::high_resolution_clock::now();
        auto lastUpdateInterval = chrono::duration_cast<chrono::milliseconds>(now - m_lastUpdate).count();


        if (lastUpdateInterval < m_updateInterval)
            continue;

        m_input.HandleEvents();


        if (m_input.MouseLBClicked() && IsMouseOverUnit(m_input.GetMousePosition()))
        {
            m_chosenUnit->SetSelected(!m_chosenUnit->IsSelected());
            m_chosenUnit = nullptr;
        }

        if (m_input.MouseRBClicked())
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
                x->SetMoveDestination(m_input.GetMousePosition());
            }
        }

        m_taskManager.HandleTasks();

        

        m_lastUpdate = now;

        for (auto & x : m_playerUnits)
        {
            auto position = x.second->GetPosition();

            SDL_Point unitSize = m_graphics.GetUnitSize();
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

            SDL_Point unitSize = m_graphics.GetUnitSize();
            SDL_Point center = { unitSize.x / 2, unitSize.y / 2 };

            position.x += counter * unitSize.x / 32;

            SDL_Rect srcRect{ 0, 0, 512, 256 };
            SDL_Rect dstRect{ position.x, position.y, unitSize.x, unitSize.y };

            m_graphics.AddToQueue("units\\placeholderAI", srcRect, dstRect, angle, center, SDL_FLIP_NONE);
            m_graphics.AddToQueue("units\\hoplites", srcRect, dstRect, 0, center, SDL_FLIP_NONE);

            if (x.second->IsSelected())
                m_graphics.AddToQueue("units\\placeholderSelected", srcRect, dstRect, angle, center, SDL_FLIP_NONE);
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
    auto unitSize = m_graphics.GetUnitSize();
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
