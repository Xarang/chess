#include "move-builder.hh"

namespace board {
 //some quick constructors for most moves

    Move MoveBuilder::BasicMove(Piece p, Position target) {
        auto m = Move();
        m.start_position_ = p.position_;
        m.end_position_ = target;
        m.piece_ = p.type_;
        return m;
    }

    Move MoveBuilder::BasicCapture(Piece p, Position target) {
        auto m = Move();
        m.start_position_ = p.position_;
        m.end_position_ = target;
        m.is_capture_ = true;
        return m;
    }

    Move MoveBuilder::DoublePawnPush(Piece p) {
        auto m = Move();
        m.start_position_ = p.position_;
        Rank newRank = p.color_ == Color::WHITE ? p.position_.rank_get() + 2 : p.position_.rank_get() - 2;
        m.end_position_ = Position(p.position_.file_get(), newRank);
        m.is_double_pawn_push_ = true;
        return m;
    }

    Move MoveBuilder::KingCastling(Piece p) {
        auto m = BasicMove(p, Position(File::G, p.position_.rank_get()));
        m.is_king_castling_ = true;
        return m;
    }

    Move MoveBuilder::QueenCastling(Piece p) {
        auto m = BasicMove(p, Position(File::B, p.position_.rank_get()));
        m.is_queen_castling_ = true;
        return m;
    }
}
   