#include "Input.h"

Input::Input()
{
}

void Input::handleEvents()
{
    SDL_Event e;

    m_mousePositionMotion.x = 0;
    m_mousePositionMotion.y = 0;

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            m_quit = true;
            break;
        }
        if (e.type == SDL_MOUSEMOTION)
        {
            if (m_mouseLBPressed)
            {
                m_mousePositionMotion.x = e.motion.x;
                m_mousePositionMotion.y = e.motion.y;
                createSelectionRectangle();
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT && e.button.state == SDL_PRESSED && e.button.clicks == 1)
            {
                m_mouseLBPressed = true;
            }

            m_mousePositionClick.x = e.button.x;
            m_mousePositionClick.y = e.button.y;

        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_LEFT && e.button.state == SDL_RELEASED && e.button.clicks == 1)
            {
                if (m_mouseLBPressed)
                    m_mouseLBWasReleased = true;
                if (m_selectionRectangle.x == 0
                    && m_selectionRectangle.y == 0
                    && m_selectionRectangle.w == 0
                    && m_selectionRectangle.h == 0)
                {
                    m_mouseLBClick = true;
                }
                
                m_mouseLBPressed = false;
            }
            else if (e.button.button == SDL_BUTTON_RIGHT && e.button.state == SDL_RELEASED && e.button.clicks == 1)
            {
                m_mouseRBClick = true;
            }

            m_mousePositionClick.x = e.button.x;
            m_mousePositionClick.y = e.button.y;

        }

        if (e.type == SDL_KEYUP)
        {
            m_keyUp = true;
            m_key = e.key.keysym.sym;
        }
    }
}

void Input::createSelectionRectangle()
{
    m_selectionRectangle = {m_mousePositionClick.x, m_mousePositionClick.y, m_mousePositionMotion.x - m_mousePositionClick.x, m_mousePositionMotion.y - m_mousePositionClick.y};
}

bool Input::getMouseLBWasReleased()
{
    if (m_mouseLBWasReleased)
    {
        m_mouseLBWasReleased = false;
        return true;
    }

    return false;
}

bool Input::getMouseLBClick()
{
    if (m_mouseLBClick)
    {
        m_mouseLBClick = false;
        return true;
    }
    
    return false;
}

bool Input::getKeyUp()
{
    if (m_keyUp)
    {
        m_keyUp = false;
        return true;
    }

    return false;
}
