#include "piece-move-builder.hh"

namespace board {
 //some quick constructors for most moves

    Move MoveBuilder::basic_move(const Piece& p, Position target) {
        auto m = Move();
        m.start_position_ = p.position_;
        m.end_position_ = target;
        m.piece_ = p.type_;
        return m;
    }

    Move MoveBuilder::basic_capture(const Piece& p, Position target) {
        auto m = Move();
        m.start_position_ = p.position_;
        m.end_position_ = target;
        m.piece_ = p.type_;
        m.is_capture_ = true;
        return m;
    }

    Move MoveBuilder::with_promotion(const Move& m, PieceType type) {
        auto promoted = Move(m);
        promoted.promotion_ = opt_piecetype_t(type);
        return promoted;
    }

    Move MoveBuilder::double_pawn_push(const Piece& p) {
        auto m = Move();
        m.start_position_ = p.position_;
        Rank newRank = p.color_ == Color::WHITE ? p.position_.rank_get() + 2 : p.position_.rank_get() - 2;
        m.end_position_ = Position(p.position_.file_get(), newRank);
        m.is_double_pawn_push_ = true;
        return m;
    }

    Move MoveBuilder::king_castling(const Piece& p) {
        auto m = basic_move(p, Position(File::G, p.position_.rank_get()));
        m.is_king_castling_ = true;
        return m;
    }

    Move MoveBuilder::queen_castling(const Piece& p) {
        auto m = basic_move(p, Position(File::C, p.position_.rank_get()));
        m.is_queen_castling_ = true;
        return m;
    }

    std::vector<Move> MoveBuilder::generate_castling_decomposition(const Piece& p, bool king_castling) {
        int direction = king_castling ? +1 : -1;
        File end_file = king_castling ? File::G : File::C;
        std::vector<Move> moves;
        for (File f = p.position_.file_get(); f != end_file; f = f + direction) {
            auto move = Move();
            move.start_position_ = Position(f, p.position_.rank_get());
            move.end_position_ = Position(f + direction, p.position_.rank_get());
            move.piece_ = PieceType::KING;
            moves.emplace_back(move); //back, will pop them from front
        }
        return moves;
    }
}
   