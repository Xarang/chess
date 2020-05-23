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
        : position_(newPos), color_(newColor), type_(newPieceType),
        has_already_moved_(false)
        {}

        void getAllPotentialMoves(std::list<struct Move>& m);

        bool operator==(const Piece& other) const {
            return position_ == other.position_ && color_ == other.color_
            && type_ == other.type_
            && has_already_moved_ == other.has_already_moved_;
        }

        char to_char_fen() const;
        static std::pair<PieceType, Color>
            piecetype_and_color_from_fen(char fen);


        std::string to_string() const;
    };

    class PieceMoveGenerator {
        public:
        static void Pawn(const Piece &p, std::list<Move>& m);
        static void Knight(const Piece &p, std::list<Move>& m);
        static void Bishop(const Piece &p, std::list<Move>& m);
        static void Rook(const Piece &p, std::list<Move>& m);
        static void Queen(const Piece &p, std::list<Move>& m);
        static void King(const Piece &p, std::list<Move>& m);
    };


}
