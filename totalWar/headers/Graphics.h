#pragma once

#include <memory>
#include <map>
#include <string>
#include <filesystem>
#include <queue>
#include <iostream>
#include <math.h>

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
        SDL_Window* m_window{ SDL_CreateWindow("nonTotalWar", 100, 100, settings::WINDOW_WIDTH, settings::WINDOW_HEIGHT, 0) };
        SDL_Renderer* m_renderer{ SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED) };

        std::map<std::string, SDL_Texture*> m_textures;

        void LoadTextures();
        std::string GetTexturePath(std::string textureName);

        struct RenderQueueItem
        {
            SDL_Texture* texture;
            SDL_Rect srcRect;
            SDL_Rect dstRect;
            double angle;
            SDL_Point center;
            SDL_RendererFlip flip;
        };

        std::queue<RenderQueueItem> m_renderQueue;
        bool m_drawSelectionRect{ false };
        SDL_Rect m_selectionRect{ 0, 0, 0, 0 };

        static bool m_drawDebugPoints;
        static std::vector<SDL_Point> m_debugPoints;

        std::string GetExePath();

    public:
        Graphics();

        void AddToQueue(std::string texture, SDL_Rect srcRect, SDL_Rect dstRect, double angle, SDL_Point center, SDL_RendererFlip flip);
        void AddSelectionRectToQueue(SDL_Rect dstRect);
        static void DebugDrawPoint(SDL_Point position);
        void RenderFrame();
    };

    inline std::string Graphics::GetTexturePath(std::string textureName)
    {
        return "resources\\" + textureName + ".png";
    }
}
