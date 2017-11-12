/** \file
    Various utility functions such as math formulas or string splitting.
*/

#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <math.h>

#include <SDL.h>

#include "Settings.h"

/**
    Splits string passed to it into a vector of strings, strings are split using char provided as splitter argument.
*/
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

/**
    Returns angle from one SDL_Point to another.
*/
inline double getAngleToPoint(const SDL_Point first, const SDL_Point second)
{
    return std::atan2(second.x - first.x, second.y - first.y) * (180.0 / 3.14159) * - 1.0;
}

/**
    Returns distance between two SDL_Point arguments.
*/
inline double getDistanceToPoint(const SDL_Point first, const SDL_Point second)
{
    return std::sqrt(std::pow(second.x - first.x, 2.0) + std::pow(second.y - first.y, 2.0));
}

/**
    Calculates the area of a triangle using Heron's formula. (only side lengths are needed)
*/
inline double getTriangleArea(const double a, const double b, const double c)
{
    auto s = (a + b + c) / 2;
    return std::sqrt(s * (s - a) * (s - b) * (s - c)); 
}

/**
    A vector of two floats, x and y, implicit conversion to SDL_Point is provided.
*/
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
