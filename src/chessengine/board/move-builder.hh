#pragma once

#include "move.hh"
#include "piece.hh"


namespace board {
    class MoveBuilder {
        public:
            static Move BasicMove(Piece p, Position target);
            static Move BasicCapture(Piece p, Position target);
            static Move DoublePawnPush(Piece p);
            static Move KingCastling(Piece p);
            static Move QueenCastling(Piece p);
    };
}