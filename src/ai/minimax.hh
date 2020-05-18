#pragma once

#include "chessengine/board/piece-type.hh"
#include <map>
#include <time.h>
#include "chessengine/board/chessboard.hh"

namespace ai {
    struct AI {
        friend board::Chessboard;
        board::Chessboard myBoard;
        clock_t remaining_time_ = 5000;
        int depth_ = 2;
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
        std::map<board::PieceType, int (*)[8][8]> table_valuesWhite = {
                {board::PieceType::QUEEN,  &queenTable},
                {board::PieceType::ROOK,   &rookTableWhite},
                {board::PieceType::BISHOP, &bishopTable},
                {board::PieceType::KNIGHT, &knightTable},
                {board::PieceType::PAWN,   &pawnTableWhite},
                {board::PieceType::KING,   &kingMiddleTableWhite}
        };

        std::map<board::PieceType, int (*)[8][8]> table_valuesBlack = {
                {board::PieceType::QUEEN,  &queenTable},
                {board::PieceType::ROOK,   &rookTableBlack},
                {board::PieceType::BISHOP, &bishopTable},
                {board::PieceType::KNIGHT, &knightTable},
                {board::PieceType::PAWN,   &pawnTableBlack},
                {board::PieceType::KING,   &kingMiddleTableBlack}
        };

        int pawnTableWhite[8][8] = {
                {0,  0,  0,  0,  0,  0,  0,  0},
                {50, 50, 50, 50, 50, 50, 50, 50},
                {10, 10, 20, 30, 30, 20, 10, 10},
                {5,  5, 10, 25, 25, 10,  5,  5},
                {0,  0,  0, 20, 20,  0,  0,  0},
                {5, -5,-10,  0,  0,-10, -5,  5},
                {5, 10, 10,-20,-20, 10, 10,  5},
                {0,  0,  0,  0,  0,  0,  0,  0}
        };

        int pawnTableBlack[8][8] = {
                {0,  0,  0,  0,  0,  0,  0,  0},
                {5, 10, 10,-20,-20, 10, 10,  5},
                {5, -5,-10,  0,  0,-10, -5,  5},
                {0,  0,  0, 20, 20,  0,  0,  0},
                {5,  5, 10, 25, 25, 10,  5,  5},
                {10, 10, 20, 30, 30, 20, 10, 10},
                {50, 50, 50, 50, 50, 50, 50, 50},
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

        int rookTableWhite[8][8] = {
                {0,  0,  0,  0,  0,  0,  0,  0},
                {5, 10, 10, 10, 10, 10, 10,  5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {0,  0,  0,  5,  5,  0,  0,  0}
        };

        int rookTableBlack[8][8] = {
                {0,  0,  0,  5,  5,  0,  0,  0},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {-5,  0,  0,  0,  0,  0,  0, -5},
                {5, 10, 10, 10, 10, 10, 10,  5},
                {0,  0,  0,  0,  0,  0,  0,  0}
        };

        int queenTable[8][8] = {
                {-20,-10,-10, -5, -5,-10,-10,-20},
                {-10,  0,  0,  0,  0,  0,  0,-10},
                {-10,  0,  5,  5,  5,  5,  0,-10},
                {-5,  0,  5,  5,  5,  5,  0, -5},
                {-5,  0,  5,  5,  5,  5,  0, -5},
                {-10,  5,  5,  5,  5,  5,  0,-10},
                {-10,  0,  5,  0,  0,  0,  0,-10},
                {-20,-10,-10, -5, -5,-10,-10,-20}
        };

        int kingMiddleTableWhite[8][8] = {
                {-30,-40,-40,-50,-50,-40,-40,-30},
                {-30,-40,-40,-50,-50,-40,-40,-30},
                {-30,-40,-40,-50,-50,-40,-40,-30},
                {-30,-40,-40,-50,-50,-40,-40,-30},
                {-20,-30,-30,-40,-40,-30,-30,-20},
                {-10,-20,-20,-20,-20,-20,-20,-10},
                {20, 20,  0,  0,  0,  0, 20, 20},
                {20, 30, 10,  0,  0, 10, 30, 20}
        };

        int kingMiddleTableBlack[8][8] = {
                {20,30,10,0,0,10,30,20},
                {20,20,10,0,0,10,30,20},
                {-10,-20,-20,-20,-20,-20,-20,-10},
                {-20,-30,-30,-40,-40,-30,-30,-20},
                {-30,-40,-40,-50,-50,-40,-40,-30},
                {-30,-40,-40,-50,-50,-40,-40,-30},
                {-30,-40,-40,-50,-50,-40,-40,-30},
                {-30,-40,-40,-50,-50,-40, -40, -30}
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

        //King Safety StockFish
        const int SafetyTable[100] = {
                0,  0,   1,   2,   3,   5,   7,   9,  12,  15,
                18,  22,  26,  30,  35,  39,  44,  50,  56,  62,
                68,  75,  82,  85,  89,  97, 105, 113, 122, 131,
                140, 150, 169, 180, 191, 202, 213, 225, 237, 248,
                260, 272, 283, 295, 307, 319, 330, 342, 354, 366,
                377, 389, 401, 412, 424, 436, 448, 459, 471, 483,
                494, 500, 500, 500, 500, 500, 500, 500, 500, 500,
                500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
                500, 500, 500, 500, 500, 500, 500, 500, 500, 500,
                500, 500, 500, 500, 500, 500, 500, 500, 500, 500
        };

        // Best Openings
        std::list<std::string> openingWhite = {"c2c4", "g1f3", "b2b3"};
        std::list<std::string> openingBlack = { "d7d5"};

        //BackwardPawnCheck
        int backwardPawnCheck(board::Position myPos);

        //CandidatePawnCheck
        int candidatePawnCheck(board::Position myPos);


        /*
         * For mobility values, we might need a specific class in order to take into account multiple paramters:
         * Value of going forward > Value of going backwards
         * Population near the possible cells we can move to
         */
        int evaluate();
        float minimax(int depth, bool is_black);
        board::Move searchMove();
    };
}
