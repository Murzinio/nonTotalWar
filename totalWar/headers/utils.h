#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <math.h>

#include <SDL.h>

#include "Settings.h"

namespace nonTotalWar
{
    inline std::vector<std::string> splitString(const std::string input, const char splitter)
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

    inline double getAngleToPoint(const SDL_Point first, const SDL_Point second)
    {
        return std::atan2(second.x - first.x, second.y - first.y) * (180.0 / 3.14159) * - 1.0;
    }

    inline double getDistanceToPoint(const SDL_Point first, const SDL_Point second)
    {
        return std::sqrt(std::pow(second.x - first.x, 2.0) + std::pow(second.y - first.y, 2.0));
    }

    inline double getTriangleArea(const double a, const double b, const double c)
    {
        // using Heron's formula
        auto s = (a + b + c) / 2;
        return std::sqrt(s * (s - a) * (s - b) * (s - c)); 
    }

    struct Vector2D
    {
        float x;
        float y;

        operator SDL_Point() const
        {
            SDL_Point point = { static_cast<int>(x), static_cast<int>(y) };
            return point;
        }
    };
}
