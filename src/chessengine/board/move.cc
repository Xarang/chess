#include "move.hh"

namespace board
{
    //take the informations we want from pgnmove and re-reformate them in a handier way.
    Move::Move(PgnMove move)
        :   piece_(move.piece_get()),
            end_position_(move.end_get()),
            start_position_(),
            promotion_(move.promotion_get()),
            is_capture_(move.capture_get()),
            is_check_(move.report_get() != ReportType::NONE),
            is_checkmate_(move.report_get() == ReportType::CHECKMATE), 
            is_double_pawn_push_(false), is_king_castling_(false), is_queen_castling_(false), is_en_passant_(false) {


        //need to figure out start position as well as special cases
        throw "not implemented";
    }
}