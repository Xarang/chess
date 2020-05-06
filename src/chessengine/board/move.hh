#pragma once

#include "chessboard.hh"
#include "../pgn/pgn-move.hh"

namespace board
{
    struct Move {
        PieceType piece_;
        Position end_position_;
        Color color_;

        std::optional<Position> start_position_;
        std::optional<PieceType> promotion_;
        std::optional<PieceType> capture_;
        std::optional<bool> double_pawn_push_;
        std::optional<bool> king_castling_;
        std::optional<bool> queen_castling_;
        std::optional<bool> en_passant_;

        Move(PgnMove pgnmove); //TODO: code this in move.cc
    };
}