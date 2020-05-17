#pragma once

#include <vector>

#include "move.hh"
#include "piece.hh"


namespace board {
    class MoveBuilder {
        public:
            static Move basic_move(Piece p, Position target);
            static Move basic_capture(Piece p, Position target);
            static Move with_promotion(Move m, PieceType type);
            static Move double_pawn_push(Piece p);
            static Move king_castling(Piece p);
            static Move queen_castling(Piece p);
            static std::vector<Move> generate_castling_decomposition(Piece p, bool king_castling);
    };
}