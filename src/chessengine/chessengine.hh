#pragma once

#include <string>
#include "board/chessboard.hh"

namespace chessengine
{
    void runPgnFile(board::Chessboard& board, std::string& filename);
}