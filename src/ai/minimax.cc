#include <cmath>
#include <algorithm>
#include <limits>
#include "chessengine/board/position.hh"
#include "ai/minimax.hh"
#include "chessengine/board/chessboard.hh"
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
                res += *table_values[piece.type_][(int)piece.position_.file_get()][(int)piece.position_.rank_get()];
            }
            else
            {
                res -= material_values[piece.type_];
                res -= *table_values[piece.type_][(int)piece.position_.file_get()][(int)piece.position_.rank_get()];
            }
        }
        return res;
    }

    static std::list<struct board::Move> getMovesFromPos(board::Position myPos, board::Chessboard& myBoard) {
        auto pieces = myBoard.get_pieces();

        for (auto piece : pieces) {
            if (piece.position_ == myPos)
                return piece.getAllPotentialMoves();
        }
        return std::list<struct board::Move>();
    }

    float AI::minimax(board::Position myPos, int depth, bool ai_turn, board::Chessboard& myBoard) {
         if (depth == 0 || myBoard.is_checkmate())
             return evaluate(myBoard);

         auto moves = getMovesFromPos(myPos, myBoard);

         // Black wants to minimize
         if (ai_turn) {
            float maxEval = -INFINITY;
            for (auto move : moves) {
                auto proj = myBoard.project(move);
                proj.change_turn();
                auto eval = minimax(move.end_position_, depth - 1, !ai_turn, proj);
                maxEval = std::max(maxEval, eval);
            }
            return maxEval;
         }

         else {
             float minEval = +INFINITY;
             for (auto move : moves) {
                 auto proj = myBoard.project(move);
                 proj.change_turn();
                 auto eval = minimax(move.end_position_, depth - 1, !ai_turn, proj);
                 minEval = std::min(minEval, eval);
             }
             return minEval;
         }
    }

    board::Move AI::searchMove(board::Chessboard& myBoard) {
        float bestValue = -INFINITY;
        board::Move bestMove;

        color_ = myBoard.whose_turn_is_it();

        auto moves = myBoard.generate_legal_moves();
        for (auto move : moves) {
            auto proj = myBoard.project(move);
            proj.change_turn();
            auto value = minimax(move.start_position_, 3, false, proj);
            if (value > bestValue)
            {
                bestValue = value;
                bestMove = move;
            }
        }
        return bestMove;
    }
}