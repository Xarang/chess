#include <list>
#include <iostream>
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
    }

    //take the informations we want from pgnmove and re-reformate them in a handier way.


    std::string Move::to_string() const {
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
        if (promotion_.has_value()) {
            tags.push_front("promoting to -> " + std::string(1, piece_to_char(promotion_.value())));
        }

        for (auto tag : tags) {
            str += " [ " + tag + " ] ";
        }
        return str;
    }

    std::string Move::uci() const {
        std::string promotion = "";
        if (promotion_.has_value()) {
            promotion = std::string(1, tolower(piece_to_char(promotion_.value())));
            //std::cerr << "promotion: " << promotion << "\n";
        }
        return start_position_.to_string() + end_position_.to_string() + promotion;
    }

    bool Move::is_halfmove_clock_resetter() const {
        return is_capture_ || piece_ == PieceType::PAWN;
    }
}