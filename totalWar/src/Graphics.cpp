#include "..\headers\Graphics.h"

using nonTotalWar::Graphics;

Graphics::Graphics()
{
    bool success = false;

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

    if (success)
        LoadTextures();
}


Graphics::~Graphics()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
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

void Graphics::AddToQueue(std::string texture, SDL_Rect srcRect, SDL_Rect dstRect, double angle, SDL_Point center, SDL_RendererFlip flip)
{
    const auto it = m_textures.find(GetTexturePath(texture));
    if (it != m_textures.cend())
        m_renderQueue.push({ it->second, srcRect, dstRect, angle, center, flip });
}

void Graphics::RenderFrame()
{
    SDL_RenderClear(m_renderer);
    while (m_renderQueue.size() > 0)
    {
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

        m_renderQueue.pop();
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

SDL_Point Graphics::GetUnitSize()
{
    return m_unitSize;
}