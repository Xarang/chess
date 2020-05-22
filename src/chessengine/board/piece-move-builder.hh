#pragma once

#include <vector>

#include "move.hh"
#include "piece.hh"


namespace board {
    class MoveBuilder {
        public:
            static Move basic_move(const Piece& p, Position target);
            static Move basic_capture(const Piece& p, Position target);
            static Move with_promotion(const Move& m, PieceType type);
            static Move double_pawn_push(const Piece& p);
            static Move king_castling(const Piece& p);
            static Move queen_castling(const Piece& p);
            static std::vector<Move> generate_castling_decomposition(const Piece& p, bool king_castling);
    };
}