#include <list>
#include "move.hh"

namespace board
{

    void Move::set_double_pawn_push() {
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

    void Move::set_king_castling() {
    if (
            piece_ == PieceType::KING
            && start_position_.file_get() == File::E
            && end_position_.file_get() == File::G
        ) {
            is_king_castling_ = true;
        }
    }

    void Move::set_queen_castling() {
    if (
            piece_ == PieceType::KING
            && start_position_.file_get() == File::E
            && end_position_.file_get() == File::C
        ) {
            is_queen_castling_ = true;
        }
    }

    void Move::set_en_passant() {
        if (piece_ == PieceType::PAWN && is_capture_
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
        set_double_pawn_push();
        set_king_castling();
        set_queen_castling();
        set_en_passant();
    }

    std::string Move::to_string() {
        std::string str = "[MOVE] ";
        str += piece_to_char(piece_);
        str += "  ";
        str += start_position_.to_string() + " -" + (is_capture_ ? "X" : "-") + "> " + end_position_.to_string();
        str += "  ";
        std::list<std::string> tags;
        if (is_king_castling_) {
            tags.push_front("king castling");
        }
        if (is_queen_castling_) {
            tags.push_front("queen castling");
        }
        if (is_double_pawn_push_) {
            tags.push_front("double pawn push");
        }
        if (is_en_passant_) {
            tags.push_front("en passant");
        }

        for (auto tag : tags) {
            str += " [ " + tag + " ] ";
        }
        return str + "\n";
    }
}