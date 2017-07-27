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
        Vector2D m_mousePosition;
    public:
        Input();
        ~Input();

        void HandleEvents();
        

        inline bool QuitRequested();
        inline bool MouseLBClicked();
        inline Vector2D GetMousePosition();
    };

    inline bool Input::QuitRequested()
    {
        return m_quit;
    }

    inline bool Input::MouseLBClicked()
    {
        if (m_mouseLBClick)
        {
            m_mouseLBClick = false;
            return true;
        }

        return false;
    }

    inline Vector2D Input::GetMousePosition()
    {
        return m_mousePosition;
    }
}

