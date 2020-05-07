#include "move.hh"
#include "../utype.hh"

namespace board
{

    void Move::setDoublePawnPush() {
        if (
                piece_ == PieceType::PAWN
                && ((
                        (start_position_.rank_get() == Rank::TWO && end_position_.rank_get() == Rank::FOUR)
                        || (start_position_.rank_get() == Rank::SEVEN && end_position_.rank_get() == Rank::FIVE)
                    ))
            ) {
                is_double_pawn_push_ = true;
                return;
            }

    }

    void Move::setKingCastling() {
    if (
            piece_ == PieceType::KING
            && start_position_.file_get() == File::E
            && end_position_.file_get() == File::G
        ) {
            is_king_castling_ = true;
        }
    }

    void Move::setQueenCastling() {
    if (
            piece_ == PieceType::KING
            && start_position_.file_get() == File::E
            && end_position_.file_get() == File::B
        ) {
            is_queen_castling_ = true;
        }
    }

    void Move::setEnPassant() {
        if (piece_ == PieceType::PAWN
            && (
                (
                    start_position_.rank_get() == Rank::FIVE
                    && end_position_.rank_get() == Rank::SIX
                    && (start_position_.file_get() + 1 == end_position_.file_get() || start_position_.file_get() - 1 == end_position_.file_get())
                )
                ||
                (
                    start_position_.rank_get() == Rank::FOUR
                    && end_position_.rank_get() == Rank::THREE
                    && (start_position_.file_get() + 1 == end_position_.file_get() || start_position_.file_get() - 1 == end_position_.file_get())
                )
            )) {
                is_en_passant_ = true;
            }
    }



    //some quick constructors for most moves

    Move Move::BasicMove(Piece p, Position target) {
        auto m = Move();
        m.start_position_ = p.position_;
        m.end_position_ = target;
        m.piece_ = p.type_;
        return m;
    }

    Move Move::BasicCapture(Piece p, Position target) {
        auto m = Move();
        m.start_position_ = p.position_;
        m.end_position_ = target;
        m.is_capture_ = true;
        return m;
    }

    Move DoublePawnPush(Piece p) {
        auto m = Move();
        m.start_position_ = p.position_;
        Rank newRank = p.color_ == Color::WHITE ? p.position_.rank_get() + 2 : p.position_.rank_get() - 2;
        m.end_position_ = Position(p.position_.file_get(), newRank);
        m.is_double_pawn_push_ = true;
        return m;
    }

    Move KingCastling(Piece p) {
        auto m = Move::BasicMove(p, Position(File::G, p.position_.rank_get()));
        m.is_king_castling_ = true;
        return m;
    }

    Move QueenCastling(Piece p) {
        auto m = Move::BasicMove(p, Position(File::B, p.position_.rank_get()));
        m.is_queen_castling_ = true;
        return m;
    }


    //take the informations we want from pgnmove and re-reformate them in a handier way.
    Move::Move(PgnMove move)
        :   piece_(move.piece_get()),
            start_position_(move.start_get()),
            end_position_(move.end_get()),
            promotion_(move.promotion_get()),
            is_capture_(move.capture_get()),
            is_check_(move.report_get() != ReportType::NONE),
            is_checkmate_(move.report_get() == ReportType::CHECKMATE), 
            is_double_pawn_push_(false), is_king_castling_(false), is_queen_castling_(false), is_en_passant_(false) {

        // detecting special moves..
        setDoublePawnPush();
        setKingCastling();
        setQueenCastling();
        setEnPassant();
    }
}