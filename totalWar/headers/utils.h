#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <math.h>

#include <SDL.h>

#include "..\Settings.h"

namespace nonTotalWar
{
    inline std::vector<std::string> SplitString(std::string input, char splitter)
    {
        std::stringstream strStream(input);
        std::string segment;
        std::vector<std::string> seglist;

        while (std::getline(strStream, segment, splitter))
        {
            seglist.push_back(segment);
        }

        return seglist;
    }

    inline double GetAngleToPoint(SDL_Point first, SDL_Point second)
    {
        return std::atan2(second.x - first.x, second.y - first.y) * (180.0 / 3.14159) * - 1.0;
    }

    inline double GetDistanceToPoint(SDL_Point first, SDL_Point second)
    {
        return std::sqrt(std::pow(second.x - first.x, 2.0) + std::pow(second.y - first.y, 2.0));
    }

    inline constexpr SDL_Point GetUnitSize()
    {
        return { settings::WINDOW_WIDTH / 24, settings::WINDOW_HEIGHT / 24 };
    }

    inline double GetTriangleArea(double a, double b, double c)
    {
        // using Heron's formula
        auto s = (a + b + c) / 2;
        return std::sqrt(s * (s - a) * (s - b) * (s - c)); 
    }
}
