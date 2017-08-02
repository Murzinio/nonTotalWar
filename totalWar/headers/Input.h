#pragma once
#include <SDL.h>
#include "utils.h"
#include <iostream>

namespace nonTotalWar
{
    class Input
    {
    private:
        bool m_quit{ false };
        bool m_mouseLBClick{ false };
        bool m_mouseRBClick{ false };
        bool m_mouseLBPressed{ false };
        bool m_mouseLBWasReleased{ false };
        SDL_Point m_mousePositionMotion;
        SDL_Point m_mousePositionClick;
        SDL_Rect m_selectionRectangle;

        void CreateSelectionRectangle();

    public:
        Input();

        void HandleEvents();

        bool GetQuitRequested() { return m_quit; };
        bool GetMouseLBPressed() { return m_mouseLBPressed; };
        bool GetMouseLBWasReleased();
        SDL_Rect GetSelectionRectangle() { return m_selectionRectangle; };
        void ClearSelectionRectangle() { m_selectionRectangle = { 0, 0, 0, 0 }; };
        bool GetMouseLBClick();
        bool GetMouseRBClicked()
        {
            if (m_mouseRBClick)
            {
                m_mouseRBClick = false;
                return true;
            }

            return false;
        };

        SDL_Point GetMousePositionClick() { return m_mousePositionClick; };
    };
}

