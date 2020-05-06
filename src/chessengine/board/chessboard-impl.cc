#include "chessboard.hh"

namespace board {

    void ChessboardImpl::do_move(Move) {
        throw "not implemented";
    }

    bool ChessboardImpl::is_move_legal(Move) {
        throw "not implemented";
    }

    bool ChessboardImpl::is_check() {
        throw "not implemented";
    }

    bool ChessboardImpl::is_checkmate() {
        throw "not implemented";
    }

    bool ChessboardImpl::is_draw() {
        throw "not implemented";
    }

    std::optional<Piece> ChessboardImpl::operator[](Position) {
        throw "not implemented";
    }

    std::string ChessboardImpl::to_string() {
        throw "not implemented";
    }
}
