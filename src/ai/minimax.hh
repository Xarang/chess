#pragma once

#include "chessengine/board/piece-type.hh"
#include <map>
#include "chessengine/board/chessboard.hh"

namespace ai {
    struct AI {
        friend board::Chessboard;
        board::Color color_ = board::Color::WHITE;

        //Piece Material Values
        std::map<board::PieceType, int> material_values = {
                {board::PieceType::QUEEN,  900},
                {board::PieceType::ROOK,   500},
                {board::PieceType::BISHOP, 330},
                {board::PieceType::KNIGHT, 320},
                {board::PieceType::PAWN,   100},
                {board::PieceType::KING,   20000}
        };

        //Piece Square Values
        std::map<board::PieceType, int (*)[8][8]> table_values = {
                {board::PieceType::QUEEN,  &queenTable},
                {board::PieceType::ROOK,   &rookTable},
                {board::PieceType::BISHOP, &bishopTable},
                {board::PieceType::KNIGHT, &knightTable},
                {board::PieceType::PAWN,   &pawnTable},
                {board::PieceType::KING,   &kingMiddleTable}
        };
        int pawnTable[8][8] = {
                {0,  0,  0,  0,  0,  0,  0,  0},
                {50, 50, 50, 50, 50, 50, 50, 50},
                {10, 10, 20, 30, 30, 20, 10, 10},
                {5,  5, 10, 25, 25, 10,  5,  5},
                {0,  0,  0, 20, 20,  0,  0,  0},
                {5, -5,-10,  0,  0,-10, -5,  5},
                {5, 10, 10,-20,-20, 10, 10,  5},
                {0,  0,  0,  0,  0,  0,  0,  0}
        };

        int knightTable[8][8] = {
                {-50,-40,-30,-30,-30,-30,-40,-50},
                {-40,-20,  0,  0,  0,  0,-20,-40},
                {-30,  0, 10, 15, 15, 10,  0,-30},
                {-30,  5, 15, 20, 20, 15,  5,-30},
                {-30,  0, 15, 20, 20, 15,  0,-30},
                {-30,  5, 10, 15, 15, 10,  5,-30},
                {-40,-20,  0,  5,  5,  0,-20,-40},
                {-50,-40,-30,-30,-30,-30,-40,-50}
        };

        int bishopTable[8][8] = {
                {-20,-10,-10,-10,-10,-10,-10,-20},
                {-10,  0,  0,  0,  0,  0,  0,-10},
                {-10,  0,  5, 10, 10,  5,  0,-10},
                {-10,  5,  5, 10, 10,  5,  5,-10},
                {-10,  0, 10, 10, 10, 10,  0,-10},
                {-10, 10, 10, 10, 10, 10, 10,-10},
                {-10,  5,  0,  0,  0,  0,  5,-10},
                {-20,-10,-10,-10,-10,-10,-10,-20}
        };

        int rookTable[8][8] = {
                {0,  0,  0,  0,  0,  0,  0,  0},
                {5, 10, 10, 10, 10, 10, 10,  5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {0,  0,  0,  5,  5,  0,  0,  0}
        };

        int queenTable[8][8] = {
                {-20,-10,-10, -5, -5,-10,-10,-20},
                {-10,  0,  0,  0,  0,  0,  0,-10},
                {-10,  0,  5,  5,  5,  5,  0,-10},
                {-5,  0,  5,  5,  5,  5,  0, -5},
                {0,  0,  5,  5,  5,  5,  0, -5},
                {-10,  5,  5,  5,  5,  5,  0,-10},
                {-10,  0,  5,  0,  0,  0,  0,-10},
                {-20,-10,-10, -5, -5,-10,-10,-20}
        };

        int kingMiddleTable[8][8] = {
                {-30,-40,-40,-50,-50,-40,-40,-30},
                {-30,-40,-40,-50,-50,-40,-40,-30},
                {-30,-40,-40,-50,-50,-40,-40,-30},
                {-30,-40,-40,-50,-50,-40,-40,-30},
                {-20,-30,-30,-40,-40,-30,-30,-20},
                {-10,-20,-20,-20,-20,-20,-20,-10},
                {20, 20,  0,  0,  0,  0, 20, 20},
                {20, 30, 10,  0,  0, 10, 30, 20}
        };

        int kingEndTable[8][8] = {
                {-50,-40,-30,-20,-20,-30,-40,-50},
                {-30,-20,-10,  0,  0,-10,-20,-30},
                {-30,-10, 20, 30, 30, 20,-10,-30},
                {-30,-10, 30, 40, 40, 30,-10,-30},
                {-30,-10, 30, 40, 40, 30,-10,-30},
                {-30,-10, 20, 30, 30, 20,-10,-30},
                {-30,-30,  0,  0,  0,  0,-30,-30},
                {-50,-30,-30,-30,-30,-30,-30,-50}
        };

        // Best Openings
        std::list<std::string> openingWhite = {"c2c4", "g1f3", "b2b3"};
        std::list<std::string> openingBlack = { "d7d5"};


        /*
         * For mobility values, we might need a specific class in order to take into account multiple paramters:
         * Value of going forward > Value of going backwards
         * Population near the possible cells we can move to
         */
        int evaluate(board::Chessboard& board);
        float minimax(board::Position myPos, int depth, bool is_black, board::Chessboard& board);
        std::string get_next_opening_move(board::Color color);
        //board::Move openingWhitesBuilder(std::string str, board::Chessboard myBoard);
        //board::Move openingBlacksBuilder(std::string str, board::Chessboard myBoard);
        board::Move searchMove(board::Chessboard& board);
    };
}
