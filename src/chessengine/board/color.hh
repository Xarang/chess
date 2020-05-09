#pragma once

namespace board
{
    /* The Color enum represent a side. */
    enum class Color : bool
    {
        WHITE = false,
        BLACK = true
    };

    std::string color_to_string(Color c) {
        if (c == Color::WHITE) {
            return "White";
        }
        if (c == Color::BLACK) {
            return "Black";
        }
        return "Unknown color";
    }
} // namespace board
