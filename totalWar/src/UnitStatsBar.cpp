#include "..\headers\UnitStatsBar.h"

using nonTotalWar::UnitStatsBar;
using nonTotalWar::Graphics;
using nonTotalWar::Unit;

UnitStatsBar::UnitStatsBar(Graphics& graphics, std::map<std::string, 
    std::shared_ptr<Unit>>& selectedUnits) : m_graphics(graphics), m_selectedUnits(selectedUnits)
{
}

void UnitStatsBar::Draw()
{
    SDL_Rect srcRect{ 0, 0, 240, 270 };
    SDL_Rect dstRect{ 0, m_size.y * 3, m_size.x, m_size.y};
    m_graphics.AddToQueue("ui\\unitStatsBar", srcRect, dstRect, 0, { 0, 0 }, SDL_FLIP_NONE);

    SDL_Point position = { 0, dstRect.y };
    position.y += dstRect.h / 10;

    auto unit = m_selectedUnits.begin()->second;
    if (unit->GetToDestroy())
        return;

    auto tasks = unit->GetTasks();
    if (tasks.size() == 0)
        m_graphics.AddTextToQueue(position, "State: Idle", { 255, 0, 0 }, 14);
    else if(unit->GetIsFighting())
        m_graphics.AddTextToQueue(position, "State: Fighting", { 255, 0, 0 }, 14);
    else
        m_graphics.AddTextToQueue(position, "State: Moving", { 255, 0, 0 }, 14);

    position.y += dstRect.h / 8;
    m_graphics.AddTextToQueue(position, "Soldiers: " + std::to_string(unit->GetSoldiers()), { 255, 0, 0 }, 14);

}