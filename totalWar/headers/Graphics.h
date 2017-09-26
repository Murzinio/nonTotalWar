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

        void loadTextures(); //TODO see why can't be set to const
        bool m_texturesLoaded{ false };
        std::string getTexturePath(const std::string textureName) const;

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

        std::string getExePath();

    public:
        Graphics();
        ~Graphics();

        //TODO use default parameters instead of overloading
        void addToQueue(const std::string texture,
            const SDL_Rect srcRect, const SDL_Rect dstRect,
            const double angle,
            const SDL_Point center,
            const SDL_RendererFlip flip,
            const bool isText); 

        void addToQueue(const std::string texture,
            const SDL_Rect srcRect, const SDL_Rect dstRect,
            const double angle,
            const SDL_Point center,
            const SDL_RendererFlip flip);

        void addSelectionRectToQueue(const SDL_Rect dstRect);
        void addTextToQueue(const SDL_Point position, 
            const std::string text, 
            SDL_Color color, 
            const int size);

        static void debugDrawPoint(const SDL_Point position);
        void renderFrame();
        bool getTexturesLoaded() const { return m_texturesLoaded; };
    };

    inline std::string Graphics::getTexturePath(const std::string textureName) const
    {
        return "resources\\" + textureName + ".png";
    };
}
