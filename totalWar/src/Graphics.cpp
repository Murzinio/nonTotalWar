#include "..\headers\Graphics.h"

using nonTotalWar::Graphics;

bool Graphics::m_drawDebugPoints = true;
std::vector<SDL_Point> Graphics::m_debugPoints = { { 0, 0 } };

Graphics::Graphics()
{
    bool success = false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    if (settings::FULLSCREEN)
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);

    if (m_window == nullptr)
    {
        std::cout << SDL_Error;
    }
    else
        success = true;

    if (m_renderer == nullptr)
    {
        std::cout << SDL_Error;
        success = false;
    }
    else
        success = true;
    TTF_Init();
    m_font = TTF_OpenFont("resources\\fonts\\verdana.ttf", 14);
    m_fontSize = 14;
    if (m_font == nullptr)
        success = false;

    std::cout << SDL_GetError();

    if (success)
    {
        LoadTextures();
        m_texturesLoaded = true;
    }
}

Graphics::~Graphics()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    TTF_CloseFont(m_font);

    m_renderer = nullptr;
    m_window = nullptr;
    m_font = nullptr;
}

void Graphics::LoadTextures()
{
    namespace filesystem = std::experimental::filesystem;

    std::vector<std::string> paths;

    for (auto & x : filesystem::recursive_directory_iterator("resources"))
        if(x.path().string().find("png") != std::string::npos)
            paths.push_back(x.path().string());

    std::string exePath = GetExePath();

    for (auto & x : paths)
    {
        std::string path = exePath + x;

        auto surface = IMG_Load(path.c_str());

        m_textures[x] = SDL_CreateTextureFromSurface(m_renderer, surface);
        if (m_textures[x] == nullptr)
            std::cout << SDL_GetError() << std::endl << IMG_GetError() << std::endl;

        SDL_FreeSurface(surface);
    }
}

void Graphics::AddToQueue(std::string texture, SDL_Rect srcRect, SDL_Rect dstRect, double angle, SDL_Point center, SDL_RendererFlip flip, bool isText)
{
    const auto it = m_textures.find(GetTexturePath(texture));
    if (it != m_textures.cend())
        m_renderQueue.push({ it->second, srcRect, dstRect, angle, center, flip, isText });
}

void Graphics::AddToQueue(std::string texture, SDL_Rect srcRect, SDL_Rect dstRect, double angle, SDL_Point center, SDL_RendererFlip flip)
{
    AddToQueue(texture, srcRect, dstRect, angle, center, flip, false);
}

void Graphics::RenderFrame()
{
    SDL_RenderClear(m_renderer);
    while (m_renderQueue.size() > 0)
    {
        if (!m_renderQueue.front().isText)
            SDL_RenderCopyEx
            (
                m_renderer,
                m_renderQueue.front().texture,
                &m_renderQueue.front().srcRect,
                &m_renderQueue.front().dstRect,
                m_renderQueue.front().angle,
                &m_renderQueue.front().center,
                m_renderQueue.front().flip
            );
        else
            SDL_RenderCopyEx
            (
                m_renderer,
                m_renderQueue.front().texture,
                nullptr,
                &m_renderQueue.front().dstRect,
                0.0,
                nullptr,
                m_renderQueue.front().flip
            );

        m_renderQueue.pop();
    }

    if (m_drawSelectionRect)
    {
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(m_renderer, m_selectionRect.x, m_selectionRect.y, m_selectionRect.x + m_selectionRect.w, m_selectionRect.y);
        SDL_RenderDrawLine(m_renderer, m_selectionRect.x, m_selectionRect.y, m_selectionRect.x, m_selectionRect.y + m_selectionRect.h);
        SDL_RenderDrawLine(m_renderer, m_selectionRect.x, m_selectionRect.y + m_selectionRect.h, m_selectionRect.x + m_selectionRect.w, m_selectionRect.y + m_selectionRect.h);
        SDL_RenderDrawLine(m_renderer, m_selectionRect.x + m_selectionRect.w, m_selectionRect.y, m_selectionRect.x + m_selectionRect.w, m_selectionRect.y + m_selectionRect.h);

        m_drawSelectionRect = false;
        m_selectionRect = { 0, 0, 0, 0 };
    }

    if (m_drawDebugPoints)
    {
        SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        for (auto & point : m_debugPoints)
            SDL_RenderDrawPoint(m_renderer, point.x, point.y);

        m_drawDebugPoints = false;
        m_debugPoints = { { 0, 0 } };

    }

    SDL_RenderPresent(m_renderer);
}

std::string Graphics::GetExePath()
{
    HMODULE hModule = GetModuleHandleW(NULL);
    WCHAR path[MAX_PATH];
    GetModuleFileNameW(hModule, path, MAX_PATH);

    char ch[260];
    char DefChar = ' ';
    WideCharToMultiByte(CP_ACP, 0, path, -1, ch, 260, &DefChar, NULL);

    std::string strPath(ch);
    std::stringstream pathStream(strPath);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(pathStream, segment, '\\'))
    {
        seglist.push_back(segment);
    }

    seglist.erase(seglist.end() - 1, seglist.end());

    std::string finalPath{ "" };
    for (auto & x : seglist)
        finalPath += x + "\\";

    return finalPath;
}

void Graphics::AddSelectionRectToQueue(SDL_Rect dstRect)
{
    m_drawSelectionRect = true;
    m_selectionRect = dstRect;
}

void Graphics::DebugDrawPoint(SDL_Point position)
{
#ifdef _DEBUG
    m_drawDebugPoints = true;
    m_debugPoints.push_back(position);
#endif
}

void Graphics::AddTextToQueue(SDL_Point position, std::string text, SDL_Color color, int size)
{
    if (m_fontSize != size)
    {
        TTF_CloseFont(m_font);
        m_font = TTF_OpenFont("resources\\fonts\\verdana.ttf", size);
        m_fontSize = size;
    }
    
    std::map<std::string, SDL_Texture*>::const_iterator it = m_textTextures.find(text);
    if (it == m_textTextures.cend())
    {
        auto surface = TTF_RenderText_Solid(m_font, text.c_str(), color);
        auto texture = SDL_CreateTextureFromSurface(m_renderer, surface);
        m_textTextures.emplace(text, texture);
        m_renderQueue.push({ texture,{ 0, 0, 0, 0 },{ position.x, position.y, surface->w, surface->h }, 0.0,{ 0, 0 }, SDL_FLIP_NONE, true });
        SDL_FreeSurface(surface);
    }
    else
    {
        int width;
        int height;
        Uint32 format = SDL_PIXELFORMAT_UNKNOWN;
        int access = SDL_TEXTUREACCESS_STATIC;
        SDL_QueryTexture(it->second, &format, &access, &width, &height);
        m_renderQueue.push({ it->second,{ 0, 0, 0, 0 },{ position.x, position.y, width, height }, 0.0,{ 0, 0 }, SDL_FLIP_NONE, true });
    }

}