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
#include <SDL_ttf.h>
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
        TTF_Font* m_font{ nullptr };
        int m_fontSize{ 0 };
        std::map<std::string, SDL_Texture*> m_textTextures;

        void LoadTextures();
        bool m_texturesLoaded{ false };
        std::string GetTexturePath(std::string textureName);

        struct RenderQueueItem
        {
            SDL_Texture* texture;
            SDL_Rect srcRect;
            SDL_Rect dstRect;
            double angle;
            SDL_Point center;
            SDL_RendererFlip flip;
            bool isText;
        };

        std::queue<RenderQueueItem> m_renderQueue;
        bool m_drawSelectionRect{ false };
        SDL_Rect m_selectionRect{ 0, 0, 0, 0 };

        static bool m_drawDebugPoints;
        static std::vector<SDL_Point> m_debugPoints;

        std::string GetExePath();

    public:
        Graphics();
        ~Graphics();

        void AddToQueue(std::string texture, SDL_Rect srcRect, SDL_Rect dstRect, double angle, SDL_Point center, SDL_RendererFlip flip, bool isText);
        void AddToQueue(std::string texture, SDL_Rect srcRect, SDL_Rect dstRect, double angle, SDL_Point center, SDL_RendererFlip flip);
        void AddSelectionRectToQueue(SDL_Rect dstRect);
        void AddTextToQueue(SDL_Point position, std::string text, SDL_Color color, int size);
        static void DebugDrawPoint(SDL_Point position);
        void RenderFrame();
        bool GetTexturesLoaded() { return m_texturesLoaded; };
    };

    inline std::string Graphics::GetTexturePath(std::string textureName)
    {
        return "resources\\" + textureName + ".png";
    };
}
