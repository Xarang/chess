#pragma once

#include <list>

#include "color.hh"
#include "move.hh"
#include "piece-type.hh"
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

        inline char piece_to_char_fen() {
            if (this->color_ == Color::WHITE) {
                switch (this->type_) {
                    case PieceType::PAWN:
                        return 'P';
                    case PieceType::KNIGHT:
                        return 'N';
                    case PieceType::BISHOP:
                        return 'B';
                    case PieceType::ROOK:
                        return 'R';
                    case PieceType::QUEEN:
                        return 'Q';
                    case PieceType::KING:
                        return 'K';
                    default:
                        return ' ';
                }
            }
            else {
                switch (this->type_) {
                    case PieceType::PAWN:
                        return 'p';
                    case PieceType::KNIGHT:
                        return 'n';
                    case PieceType::BISHOP:
                        return 'b';
                    case PieceType::ROOK:
                        return 'r';
                    case PieceType::QUEEN:
                        return 'q';
                    case PieceType::KING:
                        return 'k';
                    default:
                        return ' ';
                }
            }
        }
    };
    
}
