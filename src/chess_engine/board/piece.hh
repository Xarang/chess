#pragma once

#include "position.hh"
#include "color.hh"
#include "piece-type.hh"


namespace board
{
    struct Piece {
        Position position_;
        Color color_;
        PieceType type_;

        Piece(Position newPos, Color newColor, PieceType newPieceType) 
        : position_(newPos), color_(newColor), type_(newPieceType)
        {}
    };
    

}