#include "..\headers\Input.h"

using nonTotalWar::Input;

Input::Input()
{
}


Input::~Input()
{
}

void Input::HandleEvents()
{
    SDL_Event e;
    bool mouseAlreadyPressed{ false };

    while (SDL_PollEvent(&e) != 0)
    {
        std::cout << "event\n";

        if (e.type == SDL_QUIT)
        {
            m_quit = true;
            break;
        }

        if (e.type = SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_LEFT && e.button.state == SDL_RELEASED && e.button.clicks == 1)
            {
                m_mousePosition.x = e.button.x;
                m_mousePosition.y = e.button.y;
                m_mouseLBClick = true;

                mouseAlreadyPressed = true;
                std::cout << "mouse click\n";
                break;
            }
        }
    }
}