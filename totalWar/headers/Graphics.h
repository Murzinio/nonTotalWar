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
#include "Settings.h"
#include "utils.h"

class Graphics
{
private:
    SDL_Window* m_window{ SDL_CreateWindow("nonTotalWar", 
        100, 100, settings::WINDOW_WIDTH, settings::WINDOW_HEIGHT, 0) };
    SDL_Renderer* m_renderer{ SDL_CreateRenderer(m_window, -1, 
        SDL_RENDERER_ACCELERATED) };

    std::map<std::string, SDL_Texture*> m_textures;
    TTF_Font* m_font{ nullptr };
    int m_fontSize{ 0 };
    std::map<std::string, SDL_Texture*> m_textTextures;

    void loadTextures();

    bool m_texturesLoaded{ false };
    std::string getTexturePath(const std::string textureName) const {
        return "resources\\" + textureName + ".png";
    };

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

    /**
        Creates a new RenderQueueItem from provided arguments and adds it into render queue.
    */
    void addToQueue(const std::string texture,
        const SDL_Rect srcRect, const SDL_Rect dstRect,
        const double angle,
        const SDL_Point center,
        const SDL_RendererFlip flip); 

    /**
        Adds selection rectangle to render queue, position and size is specified in SDL_Rect passed as argument.
    */
    void addSelectionRectToQueue(const SDL_Rect dstRect);

    /**
        Creates a new RenderQueueItem text item from provided arguments and adds it into render queue.
    */
    void addTextToQueue(const SDL_Point position, 
        const std::string text, 
        SDL_Color color, 
        const int size);

    /**
        Adds a point rendered in debug mode at specified position.
    */
    static void debugDrawPoint(const SDL_Point position);

    /**
        Renders all elements from queue and debug point if specified. See debugDrawPoint.
    */
    void renderFrame();

    bool getTexturesLoaded() const { return m_texturesLoaded; };
};