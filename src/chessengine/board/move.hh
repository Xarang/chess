#pragma once

#include "position.hh"
#include "pgn/pgn-move.hh"

namespace board
{
    struct Move {
        //the type of the piece moved
        PieceType piece_;
        //ths position from which the piece moves
        Position start_position_;
        //the position where the piece is moved
        Position end_position_;

        //a pawn is getting promotted to a new piecetype
        opt_piecetype_t promotion_;

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

        Move() : piece_(PieceType::PAWN), start_position_(), end_position_(),
                promotion_(), is_capture_(false), is_check_(false),
                is_checkmate_(),
                is_double_pawn_push_(false), is_king_castling_(false),
                is_queen_castling_(false), is_en_passant_(false) {
                    //default, empty constructor
                }

        Move(PgnMove pgnmove);
        static Move parse_uci(std::string uci);

        std::string to_string() const;
        std::string uci() const;
        bool is_halfmove_clock_resetter() const;


        Move(const Move& other) = delete; //no copy allowed !
        Move(Move&& other) = default; //move is ok

        Move& operator=(Move&& other) = default;

        private:
        void set_double_pawn_push();
        void set_king_castling();
        void set_queen_castling();

    };
}