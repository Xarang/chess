#pragma once

#include "chessboard.hh"
#include "../pgn/pgn-move.hh"

namespace board
{
    struct Move {
        //the type of the piece moved
        PieceType piece_;
        //the position where the piece is moved
        Position end_position_;
        //ths position from which the piece moves (used in case of ambiguities)
        std::optional<Position> start_position_;
        //a pawn is getting promotted to a new piecetype
        std::optional<PieceType> promotion_;

        //the move is a capture
        bool is_capture_;

        //the move puts the opponent in check
        bool is_check_;

        //the move puts the opponent in checkmate
        bool is_checkmate_;

        //the move is the special case of "double pawn push"
        bool is_double_pawn_push_;

        //the move is the special case of "king castling"
        bool is_king_castling_;

        //the move is the special case of "queen castling"
        bool is_queen_castling_;

        //the move is the special case of "en passant"
        bool is_en_passant_;

        Move(PgnMove pgnmove); //TODO: code this in move.cc
    };
}