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

    std::map<std::string, std::string> m_textFields{ 
        { "State", ""},
        { "Soldiers", "" }
    };

    std::map<std::string, std::string> m_debugTextFields;

    void drawTextFields();
    void drawDebugTextFields();

public:
    /**
        Takes graphics and selected units by reference as arguments. Graphics is used to add the object to render queue, 
        map of selected units is used to display info on units currently selected by player.
    */
    UnitStatsBar(Graphics& graphics, UnitMap& selectedUnits);

    /**
        Adds the bar to Graphics render queue.
    */
    void draw();
        
    void addDebugTextField(const std::string name, const std::string text);
};
