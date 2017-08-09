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
}