#pragma once

#include <memory>
#include <map>
#include <string>
#include <filesystem>
#include <queue>
#include <iostream>

// GetExePath()
#include <windows.h>
#include <sstream>

// SDL
#include <SDL_image.h>

// game
#include "..\Settings.h"
#include "..\headers\utils.h"

namespace nonTotalWar
{
    class Graphics
    {
    private:
        SDL_Window* m_window{ SDL_CreateWindow("nonTotalWar", 0, 0, settings::WINDOW_WIDTH, settings::WINDOW_HEIGHT, 0) };
        SDL_Renderer* m_renderer{ SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED) };

        std::map<std::string, SDL_Texture*> m_textures;

        void LoadTextures();
        std::string GetTexturePath(std::string textureName);

        struct RenderQueueItem
        {
            SDL_Texture* texture;
            SDL_Rect srcRect;
            SDL_Rect dstRect;
        };

        std::queue<RenderQueueItem> m_renderQueue;
        

        std::string GetExePath();

        Vector2D m_unitSize;

    public:
        Graphics();
        ~Graphics();

        Vector2D GetUnitSize();
    
        void AddToQueue(std::string, SDL_Rect srcRect, SDL_Rect dstRect);
        void RenderFrame();
    };

    inline std::string Graphics::GetTexturePath(std::string textureName)
    {
        return "resources\\" + textureName + ".png";
    }
}
