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
    int AI::evaluate(board::Chessboard& myBoard) {
        int res = 0;
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
                        res += backwardPawnCheck(piece.position_, myBoard);
                        res += candidatePawnCheck(piece.position_, myBoard);
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
                        res -= backwardPawnCheck(piece.position_, myBoard);
                        res -= candidatePawnCheck(piece.position_, myBoard);
                    }
                }
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
        double duration;
        clock_t start = clock();

        color_ = myBoard.whose_turn_is_it();

        float bestValue = -INFINITY;
        board::Move bestMove;

        auto moves = myBoard.generate_legal_moves();

        for (auto move : moves) {
            auto proj = myBoard.project(move);
            auto value = minimax(move.end_position_, depth_, false, proj);
            if (value > bestValue)
            {
                bestValue = value;
                bestMove = move;
            }
        }
        duration = (clock() - start) / (double) CLOCKS_PER_SEC;
        std::cerr << "The turn took " << duration << "\n";
        remaining_time_ -= duration;
        if (duration <= 500)
            depth_ = 1;
        return bestMove;
    }

    int AI::backwardPawnCheck(board::Position myPos, board::Chessboard myboard) {
        int res = 0;
        if (color_ == board::Color::WHITE) {
            auto backLeft = myboard.read(board::Position(myPos.file_get() - 1, myPos.rank_get() - 1));
            if (backLeft.has_value() && backLeft.value().type_ == board::PieceType::PAWN)
                res += 30;
            auto backRight = myboard.read(board::Position(myPos.file_get() + 1, myPos.rank_get() - 1));
            if (backRight.has_value() && backRight.value().type_ == board::PieceType::PAWN) {
                if (res > 0)
                    res -= 10;
                res += 30;
            }
        } else {
            auto backLeft = myboard.read(board::Position(myPos.file_get() - 1, myPos.rank_get() + 1));
            if (backLeft.has_value() && backLeft.value().type_ == board::PieceType::PAWN)
                res += 10;

            auto backRight = myboard.read(board::Position(myPos.file_get() + 1, myPos.rank_get() + 1));
            if (backRight.has_value() && backRight.value().type_ == board::PieceType::PAWN) {
                if (res > 0)
                    res -= 10;
                res += 30;
            }
        }
        return res;
    }

    //Candidate Pawn should not take into account special Pieces.
    int AI::candidatePawnCheck(board::Position myPos, board::Chessboard myboard) {
        int res = 0;
        int i = 0;
        if (color_ == board::Color::WHITE) {
            while (!myboard[board::Position(myPos.file_get() + i, myPos.rank_get())].has_value()) {
                i++;
            }
            if (myPos.file_get() - i == board::File::H)
                res += 50;
        }
        else {
            while (!myboard[board::Position(myPos.file_get() - i, myPos.rank_get())].has_value()) {
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
