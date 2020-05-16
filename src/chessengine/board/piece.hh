#pragma once

#include <list>

#include "color.hh"
#include "move.hh"
#include "piece.hh"
#include "position.hh"


namespace board
{
    struct Piece {
        Position position_;
        Color color_;
        PieceType type_;
        bool has_already_moved_;

        Piece(Position newPos, Color newColor, PieceType newPieceType) 
        : position_(newPos), color_(newColor), type_(newPieceType), has_already_moved_(false)
        {}

        std::list<struct Move> getAllPotentialMoves();

        bool operator==(const Piece& other) const {
            return position_ == other.position_ && color_ == other.color_ && type_ == other.type_ && has_already_moved_ == other.has_already_moved_;
        }

        char to_char_fen() const;

        std::string to_string();
    };

    class PieceMoveGenerator {
        public:
            static std::list<Move> Pawn(Piece &p);
            static std::list<Move> Knight(Piece &p);
            static std::list<Move> Bishop(Piece &p);
            static std::list<Move> Rook(Piece &p);
            static std::list<Move> Queen(Piece &p);
            static std::list<Move> King(Piece &p);
    };


}
