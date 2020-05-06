#include "chessboard.hh"

namespace board {

    void ListBasedChessboard::do_move(Move) {
        throw "not implemented";
    }

    bool ListBasedChessboard::is_move_legal(Move) {
        throw "not implemented";
    }

    bool ListBasedChessboard::is_check() {
        throw "not implemented";
    }

    bool ListBasedChessboard::is_checkmate() {
        throw "not implemented";
    }

    bool ListBasedChessboard::is_draw() {
        throw "not implemented";
    }

    std::optional<Piece> ListBasedChessboard::operator[](Position) {
        throw "not implemented";
    }

}

