#pragma once

#include "Graphics.h"
#include "settings.h"
#include "units\unit.h"

class UnitStatsBar
{
private:
    Graphics& m_graphics;
    std::map<std::string, std::shared_ptr<Unit >>& m_selectedUnits;
    static constexpr SDL_Point m_size{ settings::WINDOW_WIDTH / 8, settings::WINDOW_HEIGHT / 4};

    std::map<std::string, std::string> m_textFields
    { { "State", ""},
        { "Soldiers", "" }
    };
    std::map<std::string, std::string> m_debugTextFields;

    void drawTextFields();
    void drawDebugTextFields();
public:
    UnitStatsBar(Graphics& graphics, 
        std::map<std::string, std::shared_ptr<Unit>>& selectedUnits);
    void draw();
        
    void addDebugTextField(const std::string name, const std::string text);
};
