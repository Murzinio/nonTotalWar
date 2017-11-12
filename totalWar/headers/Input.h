#pragma once
#include <SDL.h>
#include "utils.h"
#include <iostream>

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

    void createSelectionRectangle();
    bool m_keyUp;
    SDL_Keycode m_key;

public:
    Input();

    /**
        Polls SDL events and saves relevant information which can be accessed by getter functions.
    */
    void handleEvents();

    bool getQuitRequested() const { return m_quit; }
    bool getMouseLBPressed() const { return m_mouseLBPressed; }
    bool getMouseLBWasReleased();
    SDL_Rect getSelectionRectangle() const { return m_selectionRectangle; }
    void clearSelectionRectangle() { m_selectionRectangle = { 0, 0, 0, 0 }; }
    bool getMouseLBClick();

    /**
        If mouse right button was clicked, returns true and clears the information 
        (next call will return false if the mouse click event wasn't polled after previous call)
    */
    bool getMouseRBClicked();

    SDL_Point getMousePositionClick() const { return m_mousePositionClick; }
    bool getKeyUp();
    SDL_Keycode getKey() const { return m_key; }
};