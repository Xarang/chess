#include <cmath>
#include <algorithm>
#include <limits>
#include "chessengine/board/position.hh"
#include "ai/minimax.hh"
#include "chessengine/board/chessboard.hh"
#include "chessengine/board/color.hh"
#include "chessengine/board/move-builder.hh"
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

    board::Move AI::openingWhitesBuilder(std::string str, board::Chessboard myBoard) {
        if (str == "c4") {
            board::Position pos(board::File::C, board::Rank::TWO);
            board::Piece p = myBoard[pos].value();
            return board::MoveBuilder::basic_move(p, board::Position(board::File::C, board::Rank::FOUR));
        }
        if (str == "Nc3") {
            board::Position pos(board::File::B, board::Rank::ONE);
            board::Piece p = myBoard[pos].value();
            return board::MoveBuilder::basic_move(p, board::Position(board::File::C, board::Rank::THREE));
        }
        if (str == "Nf3") {
            board::Position pos(board::File::G, board::Rank::ONE);
            board::Piece p = myBoard[pos].value();
            return board::MoveBuilder::basic_move(p, board::Position(board::File::F, board::Rank::THREE));
        }
        if (str == "g3") {
            board::Position pos(board::File::G, board::Rank::TWO);
            board::Piece p = myBoard[pos].value();
            return board::MoveBuilder::basic_move(p, board::Position(board::File::G, board::Rank::THREE));
        }
        if (str == "Bg2") {
            board::Position pos(board::File::F, board::Rank::ONE);
            board::Piece p = myBoard[pos].value();
            return board::MoveBuilder::basic_move(p, board::Position(board::File::G, board::Rank::TWO));
        }
        else {
            board::Position pos(board::File::E, board::Rank::ONE);
            board::Piece p = myBoard[pos].value();
            return board::MoveBuilder::king_castling(p);
        }
    }

    board::Move AI::openingBlacksBuilder(std::string str, board::Chessboard myBoard) {
        if (str == "d5") {
            board::Position pos(board::File::D, board::Rank::SEVEN);
            board::Piece p = myBoard[pos].value();
            return board::MoveBuilder::basic_move(p, board::Position(board::File::D, board::Rank::FIVE));
        }
        if (str == "c6") {
            board::Position pos(board::File::C, board::Rank::SEVEN);
            board::Piece p = myBoard[pos].value();
            return board::MoveBuilder::basic_move(p, board::Position(board::File::C, board::Rank::SIX));
        }
        if (str == "Nf6") {
            board::Position pos(board::File::G, board::Rank::EIGHT);
            board::Piece p = myBoard[pos].value();
            return board::MoveBuilder::basic_move(p, board::Position(board::File::F, board::Rank::SIX));
        }
        if (str == "e6") {
            board::Position pos(board::File::E, board::Rank::SEVEN);
            board::Piece p = myBoard[pos].value();
            return board::MoveBuilder::basic_move(p, board::Position(board::File::E, board::Rank::SIX));
        }
        else {
            board::Position pos(board::File::B, board::Rank::EIGHT);
            board::Piece p = myBoard[pos].value();
            return board::MoveBuilder::basic_move(p, board::Position(board::File::D, board::Rank::SEVEN));
        }
    }


    board::Move AI::searchMove(board::Chessboard& myBoard) {

        float bestValue = -INFINITY;
        board::Move bestMove;

        color_ = myBoard.whose_turn_is_it();
        if (color_ == board::Color::WHITE && !openingWhite.empty()) {
            auto moveString = openingWhite.front();
            openingWhite.pop_front();
            return openingWhitesBuilder(moveString, myBoard);
        }
        else if (color_ == board::Color::BLACK && !openingBlack.empty()) {
            auto moveString = openingBlack.front();
            openingBlack.pop_front();
            return openingBlacksBuilder(moveString, myBoard);
        }

        auto moves = myBoard.generate_legal_moves();
        for (auto move : moves) {
            auto proj = myBoard.project(move);
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
