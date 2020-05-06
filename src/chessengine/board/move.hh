#pragma once

#include "chessboard.hh"
#include "../pgn/pgn-move.hh"

namespace board
{
    struct Move {
        PieceType piece_;
        Position end_position_;

        std::optional<Position> start_position_;
        std::optional<PieceType> promotion_;

        bool is_capture_;
        bool is_check_;
        bool is_checkmate_;
        bool is_double_pawn_push_;
        bool is_king_castling_;
        bool is_queen_castling_;
        bool is_en_passant_;

        Move(PgnMove pgnmove); //TODO: code this in move.cc
    };
}