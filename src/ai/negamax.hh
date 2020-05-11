#pragma once
#include <map>
#include "chessengine/board/piece-type.hh"
namespace ai {

    // According to Larry Kaufman's 2012 evaluation
    std::map<board::PieceType, int> material_values = {
            {board::PieceType::QUEEN, 1000},
            {board::PieceType::ROOK, 525},
            {board::PieceType::BISHOP, 350},
            {board::PieceType::KNIGHT, 350},
            {board::PieceType::PAWN, 100},
            {board::PieceType::KING, 10000}
            };

    /*
     * For mobility values, we might need a specific class in order to take into account multiple paramters:
     * Value of going forward > Value of going backwards
     * Population near the possible cells we can move to
     */


}