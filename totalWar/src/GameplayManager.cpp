#include "..\headers\GameplayManager.h"

using nonTotalWar::GameplayManager;
using nonTotalWar::Vector2D;
using nonTotalWar::Graphics;

GameplayManager::GameplayManager(Graphics& graphics) : m_graphics(graphics)
{
    CreateUnits();
    GameLoop();
}


GameplayManager::~GameplayManager()
{
}

void GameplayManager::CreateUnits()
{
    //TODO
    for (size_t i = 1; i < 4; i++)
    {
        nonTotalWar::Vector2D positionPlayer{ (int)(i * 100, i * (int)300) };
        nonTotalWar::Vector2D positionAi{ (int)(i * 100 + 400, i * 300)};

        m_playerUnits["Hoplites" + std::to_string(i)] = std::make_unique<Hoplites>(positionPlayer);
        m_aiUnits["Hoplites" + std::to_string(i)] = std::make_unique<Hoplites>(positionAi);
    }
}

void GameplayManager::GameLoop()
{
    //TODO
    bool initialPosition = true;

    while (true)
    {
        namespace chrono = std::chrono;
        auto now = chrono::high_resolution_clock::now();
        auto lastUpdateInterval = chrono::duration_cast<chrono::milliseconds>(now - m_lastUpdate).count();
        if (lastUpdateInterval < 1000)
            continue;
        
        m_lastUpdate = now;

        for (auto & x : m_playerUnits)
        {
            auto position = x.second->GetPosition();
            auto newPosition = position;
            newPosition.x += 10;

            x.second->ChangePosition(newPosition);
            position = x.second->GetPosition();

            Vector2D unitSize{ m_graphics.GetUnitSize().x, m_graphics.GetUnitSize().y };

            SDL_Rect srcRect{ 0, 0, unitSize.x, unitSize.y };
            SDL_Rect dstRect{ position.x, position.y, unitSize.x, unitSize.y };

            m_graphics.AddToQueue("units\\placeholderPlayer", srcRect, dstRect);
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
    std::map<std::string, std::unique_ptr<Unit>>::const_iterator it{ m_playerUnits.find(name) };
    if (it == m_playerUnits.cend())
        return false;

    it = m_aiUnits.find(name);
    if (it == m_aiUnits.cend())
        return false;

    return true;
}