#include <cmath>
#include <algorithm>
#include <limits>
#include "chessengine/board/position.hh"
#include "ai/minimax.hh"
#include "chessengine/board/chessboard.hh"
#include "chessengine/board/color.hh"
#include "chessengine/board/piece-move-builder.hh"

namespace ai {
    int AI::evaluate(board::Chessboard& myBoard) {
        int res = 0;
        auto pieces = myBoard.get_pieces();
        for (auto piece : pieces)
        {
            if (color_ == piece.color_)
            {
                //Individual piece value
                res += material_values[piece.type_];
                //Piece's positional value
                res += (*table_values[piece.type_])[(int)piece.position_.file_get()][(int)piece.position_.rank_get()];
            }
            else
            {
                res -= material_values[piece.type_];
                res -= (*table_values[piece.type_])[(int)piece.position_.file_get()][(int)piece.position_.rank_get()];
            }
        }
        return res;
    }

    float AI::minimax(board::Position myPos, int depth, bool ai_turn, board::Chessboard& myBoard) {
        (void) myPos;
         if (depth == 0 || myBoard.is_checkmate())
             return evaluate(myBoard);

         auto moves = myBoard.generate_legal_moves();

         // Black wants to minimize
         if (ai_turn) {
             float maxEval = -INFINITY;
             for (auto move : moves) {
                 auto proj = myBoard.project(move);
                 auto eval = minimax(move.end_position_, depth - 1, !ai_turn, proj);
                 maxEval = std::max(maxEval, eval);
             }
             return maxEval;
         }

         else {
             float minEval = +INFINITY;
             for (auto move : moves) {
                 auto proj = myBoard.project(move);
                 auto eval = minimax(move.end_position_, depth - 1, !ai_turn, proj);
                 minEval = std::min(minEval, eval);
             }
             return minEval;
         }
    }

    board::Move AI::searchMove(board::Chessboard& myBoard) {

        float bestValue = -INFINITY;
        board::Move bestMove;

        auto moves = myBoard.generate_legal_moves();

        for (auto move : moves) {
            auto proj = myBoard.project(move);
            auto value = minimax(move.end_position_, 2, false, proj);
            if (value > bestValue)
            {
                bestValue = value;
                bestMove = move;
            }
        }
        return bestMove;
    }
}
