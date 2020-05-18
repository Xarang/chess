#include <cmath>
#include <algorithm>
#include <limits>
#include <ctime>
#include "chessengine/board/position.hh"
#include "ai/minimax.hh"
#include "chessengine/board/chessboard.hh"
#include "chessengine/board/color.hh"
#include "chessengine/board/piece-move-builder.hh"

namespace ai {
    int AI::evaluate() {
        int res = 0;
        nb_eval += 1;

        auto pieces = myBoard.get_pieces();
        for (auto piece_pair : pieces)
        {
            for (auto piece : piece_pair.second)
            {
                if (color_ == piece.color_)
                {
                    //Individual piece value
                    res += material_values[piece.type_];
                    //Piece's positional value
                    if (color_ == board::Color::WHITE)
                        res += (*table_valuesWhite[piece.type_])[(int)piece.position_.file_get()][(int)piece.position_.rank_get()];
                    else
                        res += (*table_valuesBlack[piece.type_])[(int)piece.position_.file_get()][(int)piece.position_.rank_get()];

                    if (piece.type_ == board::PieceType::PAWN) {
                        res += backwardPawnCheck(piece.position_);
                        res += candidatePawnCheck(piece.position_);
                    }
                }
                else
                {
                    res -= material_values[piece.type_];
                    if (color_ == board::Color::WHITE)
                        res -= (*table_valuesWhite[piece.type_])[(int)piece.position_.file_get()][(int)piece.position_.rank_get()];
                    else
                        res -= (*table_valuesBlack[piece.type_])[(int)piece.position_.file_get()][(int)piece.position_.rank_get()];
                    if (piece.type_ == board::PieceType::PAWN) {
                        res -= backwardPawnCheck(piece.position_);
                        res -= candidatePawnCheck(piece.position_);
                    }
                }
            }
        }
        return res;
    }

    float AI::minimax(int depth, bool ai_turn, float alpha, float beta) {
         if (depth == 0 || myBoard.is_checkmate())
             return evaluate();

         auto moves = myBoard.generate_legal_moves();

         // Ai wants to minimize
         if (ai_turn) {
             float maxEval = -INFINITY;
             for (auto move : moves) {
                 myBoard.do_move(move, true);
                 auto eval = minimax(depth - 1, !ai_turn, alpha, beta);
                 myBoard.undo_move(move);
                 maxEval = std::max(maxEval, eval);
                 alpha = std::max(alpha, eval);
                 if (beta <= alpha)
                 {
                     break;
                 }
             }
             return maxEval;
         }

         else {
             float minEval = +INFINITY;
             for (auto move : moves) {
                 myBoard.do_move(move, true);
                 auto eval = minimax(depth - 1, !ai_turn, alpha, beta);
                 myBoard.undo_move(move);
                 minEval = std::min(minEval, eval);
                 beta = std::min(beta, eval);
                 if (beta <= alpha)
                 {
                     break;
                 }
             }
             return minEval;
         }
    }

    board::Move AI::searchMove() {
        double duration;
        clock_t start = clock();

        color_ = myBoard.whose_turn_is_it();

        float bestValue = -INFINITY;
        board::Move bestMove;

        auto moves = myBoard.generate_legal_moves();

        for (auto move : moves) {
            myBoard.do_move(move);
            auto value = minimax(depth_, false, -INFINITY, +INFINITY);
            myBoard.undo_move(move, true);
            if (value > bestValue)
            {
                bestValue = value;
                bestMove = move;
            }
        }
        duration = (clock() - start) / (double) CLOCKS_PER_SEC;
        std::cerr << "time to make move: " << duration << "\n";
        remaining_time_ -= duration;
        if (duration >= 4000)
            depth_ = 1;
        std::cerr << "nb eval = " << nb_eval << "\n";
        nb_eval = 0;
        return bestMove;
    }

    int AI::backwardPawnCheck(board::Position myPos) {
        int res = 0;
        if (color_ == board::Color::WHITE) {
            auto backLeft = myBoard.read(board::Position(myPos.file_get() - 1, myPos.rank_get() - 1));
            if (backLeft.has_value() && backLeft.value().type_ == board::PieceType::PAWN)
                res += 30;
            auto backRight = myBoard.read(board::Position(myPos.file_get() + 1, myPos.rank_get() - 1));
            if (backRight.has_value() && backRight.value().type_ == board::PieceType::PAWN) {
                if (res > 0)
                    res -= 10;
                res += 30;
            }
        } else {
            auto backLeft = myBoard.read(board::Position(myPos.file_get() - 1, myPos.rank_get() + 1));
            if (backLeft.has_value() && backLeft.value().type_ == board::PieceType::PAWN)
                res += 10;

            auto backRight = myBoard.read(board::Position(myPos.file_get() + 1, myPos.rank_get() + 1));
            if (backRight.has_value() && backRight.value().type_ == board::PieceType::PAWN) {
                if (res > 0)
                    res -= 10;
                res += 30;
            }
        }
        return res;
    }

    //Candidate Pawn should not take into account special Pieces.
    int AI::candidatePawnCheck(board::Position myPos) {
        int res = 0;
        int i = 0;
        if (color_ == board::Color::WHITE) {
            while (!myBoard[board::Position(myPos.file_get() + i, myPos.rank_get())].has_value()) {
                i++;
            }
            if (myPos.file_get() - i == board::File::H)
                res += 50;
        }
        else {
            while (!myBoard[board::Position(myPos.file_get() - i, myPos.rank_get())].has_value()) {
                i++;
            }
            if (myPos.file_get() - i == board::File::A)
                res += 50;
        }
        return res;
    }

    //King Safety Stockfish
    //int AI::KingSafety

}
