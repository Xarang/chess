#include "color.hh"

namespace board {
std::string color_to_string(Color c) {
    if (c == Color::WHITE) {
        return "White";
    }
    if (c == Color::BLACK) {
        return "Black";
    }
    return "Unknown color";
    }
}