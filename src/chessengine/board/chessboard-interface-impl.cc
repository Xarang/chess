#include "pgn/pgn-parser.hh"
#include "pgn/pgn-move.hh"
#include "chessboard-interface-impl.hh"
#include "piece-type.hh"

#include "position.hh"
#include "color.hh"

namespace board {
    using side_piece_t = std::pair<PieceType, Color>;
    using opt_piece_t = std::optional<side_piece_t>; 

    opt_piece_t ChessboardInterfaceImpl::operator[](const Position& position) const {
        auto piece_ptr = cb_->read(position);
        if (piece_ptr) {
            return std::pair<PieceType, Color>(piece_ptr->type_, piece_ptr->color_);
        }
        return std::nullopt;
    }
   

}