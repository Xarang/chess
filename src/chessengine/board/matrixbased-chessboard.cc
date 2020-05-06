#include "chessboard.hh"

namespace board {

    void MatrixBasedChessboard::do_move(Move m) {
        throw "not implemented";
    }

    bool MatrixBasedChessboard::is_move_legal(Move m) {
        throw "not implemented";
    }

    bool MatrixBasedChessboard::is_check() {
        throw "not implemented";
    }

    bool MatrixBasedChessboard::is_checkmate() {
        throw "not implemented";
    }

    bool MatrixBasedChessboard::is_draw() {
        throw "not implemented";
    }

    std::optional<Piece> MatrixBasedChessboard::operator[](Position p) {
        throw "not implemented";
    }

}
