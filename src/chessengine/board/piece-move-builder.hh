#pragma once

#include <vector>

#include "move.hh"
#include "piece.hh"


namespace board {
    class MoveBuilder {
        public:
            static inline void set_basic_move(Move& move,
                    const Piece& p, Position target) {
                move.start_position_ = p.position_;
                move.end_position_ = target;
                move.piece_ = p.type_;
            }

            static inline void set_basic_capture(Move& move,
                    const Piece& p, Position target) {
                move.start_position_ = p.position_;
                move.end_position_ = target;
                move.piece_ = p.type_;
                move.is_capture_ = true;
            }

            static inline void set_with_promotion(Move& move,
                    const Move& original, PieceType type) {
                //only place where move copying should happen
                // (only copying relevant fields)
                move.start_position_ = original.start_position_;
                move.end_position_ = original.end_position_;
                move.piece_ = original.piece_;
                move.is_capture_ = original.is_capture_;
                move.is_check_ = original.is_check_;
                move.is_checkmate_ = original.is_checkmate_;
                move.promotion_ = opt_piecetype_t(type);
            }

            static inline void set_double_pawn_push(Move& move,
                    const Piece& p) {
                move.start_position_ = p.position_;
                Rank newRank = p.color_ == Color::WHITE ?
                        p.position_.rank_get() + 2 : p.position_.rank_get() - 2;
                move.end_position_ = Position(p.position_.file_get(), newRank);
                move.is_double_pawn_push_ = true;
            }

            static inline void set_king_castling(Move& m, const Piece& p) {
                MoveBuilder::set_basic_move(m, p,
                        Position(File::G, p.position_.rank_get()));
                m.is_king_castling_ = true;
            }
            static inline void set_queen_castling(Move& m, const Piece& p) {
                MoveBuilder::set_basic_move(m, p,
                        Position(File::C, p.position_.rank_get()));
                m.is_queen_castling_ = true;
            }

            static void set_castling_decomposition(std::vector<Move>&,
                    const Piece& p, bool king_castling);
    };
}