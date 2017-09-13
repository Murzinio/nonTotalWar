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

    SDL_Point position{ 0, dstRect.y };
    position.y += dstRect.h / 10;

    auto unit = m_selectedUnits.begin()->second;
    if (unit->GetToDestroy())
        return;

    auto& stateField = m_textFields.find("State")->second;
    auto tasks = unit->GetTasks();
    if (tasks.size() == 0 && !unit->GetIsFighting())
        stateField = "Idle";
    else if (unit->GetIsFighting())
        stateField = "Fighting";
    else
        stateField = "Moving";

    position.y += dstRect.h / 8;
    auto& soldiersField = m_textFields.find("Soldiers")->second;
    soldiersField = std::to_string(unit->GetSoldiers());

    auto angle = unit->GetAngle();
    auto anglec = std::to_string(angle);

    m_debugTextFields.clear();
    auto unitPosition = unit->GetPosition();
    AddDebugTextField("Position x", std::to_string(unitPosition.x));
    AddDebugTextField("Position y", std::to_string(unitPosition.y));
    AddDebugTextField("Angle", std::to_string(unit->GetAngle()));
    DrawTextFields();
    DrawDebugTextFields();
}

void UnitStatsBar::AddDebugTextField(std::string name, std::string text)
{
    m_debugTextFields.insert({ name, text });
}

void UnitStatsBar::DrawTextFields()
{
    SDL_Point position{ 0, m_size.y * 3 };
    bool first = true;
    for (auto field : m_textFields)
    {
        if (first)
        {
            m_graphics.AddTextToQueue(position, field.first + ": " + field.second, { 255, 0, 0 }, 14);
            first = false;
        }
        else
        {
            position.y += 14;
            m_graphics.AddTextToQueue(position, field.first + ": " + field.second, { 255, 0, 0 }, 14);
        }
    }
}

void UnitStatsBar::DrawDebugTextFields()
{
#ifdef _DEBUG
    SDL_Point position{ 0, (m_size.y * 3) + static_cast<int>((m_textFields.size() + 1) * 14) };

    m_graphics.AddTextToQueue(position, "DEBUG", { 255, 0, 0 }, 14);

    for (auto & field : m_debugTextFields)
    {
        position.y += 14;
        m_graphics.AddTextToQueue(position, field.first + ": " + field.second, { 255, 0, 0 }, 14);
    }
#endif
}