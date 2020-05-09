#pragma once

#include <string>

namespace board
{
    /* The Color enum represent a side. */
    enum class Color : bool
    {
        WHITE = false,
        BLACK = true
    };

    std::string color_to_string(Color c);
    
} // namespace board
