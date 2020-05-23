#include "piece-move-builder.hh"

namespace board {
 //some quick constructors for most moves

    void MoveBuilder::set_castling_decomposition(std::vector<Move>& moves,
            const Piece& p, bool king_castling) {
        int direction = king_castling ? +1 : -1;
        File end_file = king_castling ? File::G : File::C;
        for (File f = p.position_.file_get();
            f != end_file; f = f + direction) {
            moves.emplace_back(Move());
            moves.back().start_position_ = Position(f, p.position_.rank_get());
            moves.back().end_position_ = Position(f + direction,
                    p.position_.rank_get());
            moves.back().piece_ = PieceType::KING;
        }
    }
}
   