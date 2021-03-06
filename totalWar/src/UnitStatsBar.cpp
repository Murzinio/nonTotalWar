#include "UnitStatsBar.h"

UnitStatsBar::UnitStatsBar(Graphics& graphics, std::vector<Unit*>& selectedUnits) : m_graphics(graphics), m_selectedUnits(selectedUnits)
{
}

void UnitStatsBar::draw()
{
    SDL_Rect srcRect{ 0, 0, 240, 270 };
    SDL_Rect dstRect{ 0, m_size.y * 3, m_size.x, m_size.y};
    m_graphics.addToQueue("ui\\unitStatsBar", srcRect, dstRect, 0, { 0, 0 }, SDL_FLIP_NONE);

    SDL_Point position{ 0, dstRect.y };
    position.y += dstRect.h / 10;

    auto unit = *m_selectedUnits.begin();
    if (unit->getToDestroy())
        return;

    auto& stateField = m_textFields.find("State")->second;
    auto tasks = unit->getTasks();
    if (tasks.size() == 0 && !unit->getIsFighting())
        stateField = "Idle";
    else if (unit->getIsFighting())
        stateField = "Fighting";
    else
        stateField = "Moving";

    position.y += dstRect.h / 8;
    auto& soldiersField = m_textFields.find("Soldiers")->second;
    soldiersField = std::to_string(unit->getSoldiers());

    auto angle = unit->getAngle();
    auto anglec = std::to_string(angle);

    m_debugTextFields.clear();
    auto unitPosition = unit->getPosition();
    addDebugTextField("Position x", std::to_string(unitPosition.x));
    addDebugTextField("Position y", std::to_string(unitPosition.y));
    addDebugTextField("Angle", std::to_string(unit->getAngle()));
    drawTextFields();
    drawDebugTextFields();
}

void UnitStatsBar::addDebugTextField(const std::string name, const std::string text)
{
    m_debugTextFields.insert({ name, text });
}

void UnitStatsBar::drawTextFields()
{
    SDL_Point position{ 0, m_size.y * 3 };
    bool first = true;
    for (auto field : m_textFields)
    {
        if (first)
        {
            m_graphics.addTextToQueue(position, field.first + ": " + field.second, { 255, 0, 0 }, 14);
            first = false;
        }
        else
        {
            position.y += 14;
            m_graphics.addTextToQueue(position, field.first + ": " + field.second, { 255, 0, 0 }, 14);
        }
    }
}

void UnitStatsBar::drawDebugTextFields()
{
#ifdef _DEBUG
    SDL_Point position{ 0, (m_size.y * 3) + static_cast<int>((m_textFields.size() + 1) * 14) };

    m_graphics.addTextToQueue(position, "DEBUG", { 255, 0, 0 }, 14);

    for (auto & field : m_debugTextFields)
    {
        position.y += 14;
        m_graphics.addTextToQueue(position, field.first + ": " + field.second, { 255, 0, 0 }, 14);
    }
#endif
}