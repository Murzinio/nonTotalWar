#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <SDL.h>

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
        return static_cast<double>(atan2(second.x - first.x, second.y - first.y)) * (180.0f / 3.14159f) * - 1;
    }
}
