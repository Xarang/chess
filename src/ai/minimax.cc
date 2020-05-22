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
        auto pieces = myBoard.get_pieces();

        res += pair_modify(pieces);
        res += bishopEval();

        res += knight_pawns(pieces);

        for (auto piece_pair : pieces)
        {
            for (auto piece : piece_pair.second)
            {
                if (color_ == piece->color_)
                {
                    //Individual piece value
                    res += material_values[piece->type_];
                    //Piece's positional value
                    if (color_ == board::Color::WHITE)
                        res += (*table_valuesWhite[piece->type_])[(int)piece->position_.file_get()][(int)piece->position_.rank_get()];
                    else
                        res += (*table_valuesBlack[piece->type_])[(int)piece->position_.file_get()][(int)piece->position_.rank_get()];

                    if (piece->type_ == board::PieceType::PAWN) {
                        res += backwardPawnCheck(piece->position_);
                        res += candidatePawnCheck(piece->position_);
                    }
                }
                else
                {
                    res -= material_values[piece->type_];
                    if (color_ == board::Color::WHITE)
                        res -= (*table_valuesWhite[piece->type_])[(int)piece->position_.file_get()][(int)piece->position_.rank_get()];
                    else
                        res -= (*table_valuesBlack[piece->type_])[(int)piece->position_.file_get()][(int)piece->position_.rank_get()];
                    if (piece->type_ == board::PieceType::PAWN) {
                        res -= backwardPawnCheck(piece->position_);
                        res -= candidatePawnCheck(piece->position_);
                    }
                }
            }
        }
        return res;
    }

    float AI::minimax(const int& depth, const bool& ai_turn, float alpha, float beta) {
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

    int AI::pair_modify(const std::unordered_map<std::pair<board::PieceType, board::Color>, std::vector<board::Piece*>, board::hash_pair>& pieces) {
        int res = 0;
        //Bonus
        if (pieces.find(std::make_pair<board::PieceType, board::Color>(board::PieceType::BISHOP, board::Color::WHITE))->second.size() > 1)
            res += 50;
        if (pieces.find(std::make_pair<board::PieceType, board::Color>(board::PieceType::BISHOP, board::Color::BLACK))->second.size() > 1)
            res -= 50;
        //Malus
        if (pieces.find(std::make_pair<board::PieceType, board::Color>(board::PieceType::KNIGHT, board::Color::WHITE))->second.size() > 1)
            res -= 30;
        if (pieces.find(std::make_pair<board::PieceType, board::Color>(board::PieceType::KNIGHT, board::Color::BLACK))->second.size() > 1)
            res += 30;
        if (pieces.find(std::make_pair<board::PieceType, board::Color>(board::PieceType::ROOK, board::Color::WHITE))->second.size() > 1)
            res -= 40;
        if (pieces.find(std::make_pair<board::PieceType, board::Color>(board::PieceType::ROOK, board::Color::BLACK))->second.size() > 1)
            res += 40;
        if (pieces.find(std::make_pair<board::PieceType, board::Color>(board::PieceType::PAWN, board::Color::WHITE))->second.size() == 0)
            res -= 70;
        if (pieces.find(std::make_pair<board::PieceType, board::Color>(board::PieceType::PAWN, board::Color::BLACK))->second.size() == 0)
            res += 70;
        return res;
    }

    int AI::knight_pawns(const std::unordered_map<std::pair<board::PieceType, board::Color>, std::vector<board::Piece*>, board::hash_pair>& pieces) {
        int res = 0;
        int whitePawns = pieces.find(std::make_pair<board::PieceType, board::Color>(board::PieceType::PAWN, board::Color::WHITE))->second.size();
        int whiteKnights = pieces.find(std::make_pair<board::PieceType, board::Color>(board::PieceType::KNIGHT, board::Color::WHITE))->second.size();
        int blackPawns = pieces.find(std::make_pair<board::PieceType, board::Color>(board::PieceType::PAWN, board::Color::WHITE))->second.size();
        int blackKnights = pieces.find(std::make_pair<board::PieceType, board::Color>(board::PieceType::KNIGHT, board::Color::WHITE))->second.size();

        res -= (8 - whitePawns) * 15 * whiteKnights;
        res += (8 - blackPawns) * 15 * blackKnights;
        return res;
    }

    int AI::bishopEval() {
        int res = 0;
        auto whiteBishops = myBoard.get_pieces().find(std::make_pair<board::PieceType, board::Color>(board::PieceType::BISHOP, board::Color::WHITE))->second;
        auto blackBishops = myBoard.get_pieces().find(std::make_pair<board::PieceType, board::Color>(board::PieceType::BISHOP, board::Color::BLACK))->second;

        for (auto piece : whiteBishops) {
            if (piece->position_ == board::Position(board::File::A, board::Rank::SEVEN)
                && (myBoard.occupied_by(board::Color::BLACK, board::PieceType::PAWN,
                        board::Position(board::File::B, board::Rank::SIX))))
                    res -= 30;
            else if (piece->position_ == board::Position(board::File::H, board::Rank::SEVEN)
                     && (myBoard.occupied_by(board::Color::BLACK, board::PieceType::PAWN,
                             board::Position(board::File::G, board::Rank::SIX))))
                    res -= 30;
            else if (piece->position_ == board::Position(board::File::B, board::Rank::EIGHT)
                     && (myBoard.occupied_by(board::Color::BLACK, board::PieceType::PAWN,
                                             board::Position(board::File::C, board::Rank::SEVEN))))
                    res -= 30;
            else if (piece->position_ == board::Position(board::File::G, board::Rank::EIGHT)
                     && (myBoard.occupied_by(board::Color::BLACK, board::PieceType::PAWN,
                                             board::Position(board::File::F, board::Rank::SEVEN))))
                res -= 30;
            else if (piece->position_ == board::Position(board::File::A, board::Rank::SIX)
                     && (myBoard.occupied_by(board::Color::BLACK, board::PieceType::PAWN,
                                             board::Position(board::File::B, board::Rank::FIVE))))
                res -= 30;
            else if (piece->position_ == board::Position(board::File::H, board::Rank::SIX)
                     && (myBoard.occupied_by(board::Color::BLACK, board::PieceType::PAWN,
                                             board::Position(board::File::G, board::Rank::FIVE))))
                res -= 30;
            else if (piece->position_ == board::Position(board::File::F, board::Rank::ONE)
                     && (myBoard.occupied_by(board::Color::WHITE, board::PieceType::KING,
                                             board::Position(board::File::G, board::Rank::ONE))))
                res += 30;
            else if (piece->position_ == board::Position(board::File::C, board::Rank::ONE)
                     && (myBoard.occupied_by(board::Color::WHITE, board::PieceType::KING,
                                             board::Position(board::File::B, board::Rank::ONE))))
                res += 30;
            }
        for (auto piece : blackBishops) {
            if (piece->position_ == board::Position(board::File::A, board::Rank::TWO)
                && (myBoard.occupied_by(board::Color::WHITE, board::PieceType::PAWN,
                                        board::Position(board::File::B, board::Rank::THREE))))
                res += 30;
            else if (piece->position_ == board::Position(board::File::H, board::Rank::TWO)
                     && (myBoard.occupied_by(board::Color::WHITE, board::PieceType::PAWN,
                                             board::Position(board::File::G, board::Rank::THREE))))
                res += 30;
            else if (piece->position_ == board::Position(board::File::B, board::Rank::ONE)
                     && (myBoard.occupied_by(board::Color::WHITE, board::PieceType::PAWN,
                                             board::Position(board::File::C, board::Rank::TWO))))
                res += 30;
            else if (piece->position_ == board::Position(board::File::G, board::Rank::ONE)
                     && (myBoard.occupied_by(board::Color::WHITE, board::PieceType::PAWN,
                                             board::Position(board::File::F, board::Rank::TWO))))
                res += 30;
            else if (piece->position_ == board::Position(board::File::A, board::Rank::THREE)
                     && (myBoard.occupied_by(board::Color::WHITE, board::PieceType::PAWN,
                                             board::Position(board::File::B, board::Rank::FOUR))))
                res += 30;
            else if (piece->position_ == board::Position(board::File::H, board::Rank::THREE)
                     && (myBoard.occupied_by(board::Color::BLACK, board::PieceType::PAWN,
                                             board::Position(board::File::G, board::Rank::FOUR))))
                res += 30;
            else if (piece->position_ == board::Position(board::File::F, board::Rank::EIGHT)
                     && (myBoard.occupied_by(board::Color::BLACK, board::PieceType::KING,
                                             board::Position(board::File::G, board::Rank::EIGHT))))
                res -= 30;
            else if (piece->position_ == board::Position(board::File::C, board::Rank::EIGHT)
                     && (myBoard.occupied_by(board::Color::BLACK, board::PieceType::KING,
                                             board::Position(board::File::B, board::Rank::EIGHT))))
                res -= 30;
        }
        return res;
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
        remaining_time_ -= duration;
        if (remaining_time_ <= 120)
            depth_ = 2;
        return bestMove;
    }

    int AI::backwardPawnCheck(const board::Position& myPos) {
        int res = 0;
        if (color_ == board::Color::WHITE) {
            auto backLeft = myBoard.read(board::Position(myPos.file_get() - 1, myPos.rank_get() - 1));
            if (backLeft && backLeft->type_ == board::PieceType::PAWN)
                res += 30;
            auto backRight = myBoard.read(board::Position(myPos.file_get() + 1, myPos.rank_get() - 1));
            if (backRight && backRight->type_ == board::PieceType::PAWN) {
                if (res > 0)
                    res -= 10;
                res += 30;
            }
        } else {
            auto backLeft = myBoard.read(board::Position(myPos.file_get() - 1, myPos.rank_get() + 1));
            if (backLeft && backLeft->type_ == board::PieceType::PAWN)
                res += 10;

            auto backRight = myBoard.read(board::Position(myPos.file_get() + 1, myPos.rank_get() + 1));
            if (backRight && backRight->type_ == board::PieceType::PAWN) {
                if (res > 0)
                    res -= 10;
                res += 30;
            }
        }
        return res;
    }

    //Candidate Pawn should not take into account special Pieces.
    int AI::candidatePawnCheck(const board::Position& myPos) {
        int res = 0;
        int i = 0;
        if (color_ == board::Color::WHITE) {
            while (!myBoard.read(board::Position(myPos.file_get() + i, myPos.rank_get()))) {
                i++;
            }
            if (myPos.file_get() - i == board::File::H)
                res += 50;
        }
        else {
            while (!myBoard.read(board::Position(myPos.file_get() - i, myPos.rank_get()))) {
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
