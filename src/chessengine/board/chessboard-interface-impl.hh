#pragma once

#include "chessboard.hh"
#include "chessboard-interface.hh"

namespace board {

    class ChessboardInterfaceImpl : public ChessboardInterface {
        private:
            Chessboard& cb_;
        public:
            using side_piece_t = std::pair<PieceType, Color>;
            using opt_piece_t = std::optional<side_piece_t>;

            ChessboardInterfaceImpl(Chessboard& cb) : cb_(cb) {}
            ChessboardInterfaceImpl(const ChessboardInterfaceImpl& other) = default;
            ChessboardInterfaceImpl& operator=(const ChessboardInterfaceImpl &other) {
                cb_ = other.cb_;
                return *this;
            }

            /*!
            ** Access operator.
            ** @param position A position on the chessboard.
            ** @return Piece and Color, or nullopt_t if the square is empty
            */
            opt_piece_t operator[](const Position& position) const;
    };

}
