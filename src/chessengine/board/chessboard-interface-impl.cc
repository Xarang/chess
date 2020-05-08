#include "pgn/pgn-parser.hh"
#include "pgn/pgn-move.hh"
#include "chessboard-interface-impl.hh"

namespace board {
    using side_piece_t = std::pair<PieceType, Color>;
    using opt_piece_t = std::optional<side_piece_t>; 

    opt_piece_t ChessboardInterfaceImpl::operator[](const Position& position) const {
        std::optional<Piece> p = cb_[position];
        if (p.has_value()) {
            return std::pair<PieceType, Color>(p->type_, p->color_);
        }
        return std::nullopt;
    }

   

}