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

        Piece(Position newPos, Color newColor, PieceType newPieceType) 
        : position_(newPos), color_(newColor), type_(newPieceType)
        {}

        std::list<struct Move> getAllPotentialMoves();
    };
    
}