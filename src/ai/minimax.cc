#include <cmath>
#include <algorithm>
#include <limits>
#include "chessengine/board/position.hh"
#include "ai/minimax.hh"
#include "chessengine/board/chessboard.hh"



namespace ai {
    int Minimax::evaluate() {
        int res = 0;
        auto pieces = myBoard.get_pieces();
        for (auto piece : pieces)
            res += material_values[piece.type_];
        return res;
    }

    std::list<struct board::Move> getMovesFromPos(board::Position myPos, board::Chessboard myBoard) {
        auto pieces = myBoard.get_pieces();

        for (auto piece : pieces) {
            if (piece.position_ == myPos)
                return piece.getAllPotentialMoves();
        }
        return std::list<struct board::Move>();
    }

    float Minimax::minimax(board::Position myPos, int depth, bool is_black) {
         if (depth == 0 || myBoard.is_checkmate())
             return evaluate();

         auto moves = getMovesFromPos(myPos, myBoard);

         // Black wants to minimize
         if (!is_black) {
            float maxEval = -INFINITY;
            for (auto move : moves) {
                auto eval = minimax(move.end_position_, depth - 1, !is_black);
                maxEval = std::max(maxEval, eval);
            }
            return maxEval;
         }

         else {
             float minEval = +INFINITY;
             for (auto move : moves) {
                 auto eval = minimax(move.end_position_, depth - 1, !is_black);
                 minEval = std::min(minEval, eval);
             }
             return minEval;
         }
    }
    /*
    board::Move Minimax::searchMove() {
        float bestValue = -INFINITY;
        board::Move bestMove;

        auto moves = myBoard.generateLegalMoves();
        for (auto move : moves) {
            bestValue = minimax(move.start_position_, 3, false);
        }
    }
     */
}