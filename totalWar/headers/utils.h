#pragma once
#include <string>
#include <vector>
#include <sstream>

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

    struct Vector2D
    {
        int x;
        int y;
        // uses int for compatibility with SDL_Rect
    };
}
