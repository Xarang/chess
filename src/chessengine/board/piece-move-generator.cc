//
// Created by xarang on 16/05/2020.
//

#include <iostream>
#include "move.hh"
#include "piece-move-builder.hh"
#include "piece.hh"

namespace board {

    static const std::pair<int,int> knight_directions[] {
        {1, 2},
        {1, -2},
        {-1, 2},
        {-1, -2},
        {2, 1},
        {2, -1},
        {-2, 1},
        {-2, -1}
    };

    static const std::pair<int,int> bishop_directions[] {
        {1, 1},
        {-1, -1},
        {1, -1},
        {-1, 1}
    };

    static const std::pair<int,int> rook_directions[] {
            {1, 0},
            {-1, 0},
            {0, -1},
            {0, 1}
    };

    static const std::pair<int,int> queen_directions[] {
            {1, 1},
            {-1, -1},
            {1, -1},
            {-1, 1},
            {1, 0},
            {-1, 0},
            {0, -1},
            {0, 1}
    };




    std::list<Move> PieceMoveGenerator::Pawn(const Piece &p) {
        std::list<Move> moves;
        int direction = p.color_ == Color::WHITE ? 1 : -1;
        //double pawn advance
        if (!p.has_already_moved_) {
            moves.emplace_front(MoveBuilder::double_pawn_push(p));
        }
        //move forward
        moves.emplace_front(
                MoveBuilder::basic_move(p, Position(p.position_.file_get(), p.position_.rank_get() + direction)));
        //capture in diagonals
        moves.emplace_front(MoveBuilder::basic_capture(p, Position(p.position_.file_get() + 1,
                                                                p.position_.rank_get() + direction)));
        moves.emplace_front(MoveBuilder::basic_capture(p, Position(p.position_.file_get() - 1,
                                                                p.position_.rank_get() + direction)));

        for (auto move : moves) {
            if ((move.end_position_.rank_get() == Rank::EIGHT && p.color_ == Color::WHITE)
                || (move.end_position_.rank_get() == Rank::ONE && p.color_ == Color::BLACK)) {
                moves.emplace_front(MoveBuilder::with_promotion(move, PieceType::BISHOP));
                moves.emplace_front(MoveBuilder::with_promotion(move, PieceType::QUEEN));
                moves.emplace_front(MoveBuilder::with_promotion(move, PieceType::ROOK));
                moves.emplace_front(MoveBuilder::with_promotion(move, PieceType::KNIGHT));
            }
        }

        //std::cout << "generated potential moves for piece: " << p.to_string() << "\n";
        return moves;
    }

    std::list<Move> PieceMoveGenerator::Bishop(const Piece &p) {
        std::list<Move> bishop_moves;
        File org_file = p.position_.file_get();
        Rank org_rank = p.position_.rank_get();

        for (auto pair : bishop_directions) {

            File curr_file = org_file;
            Rank curr_rank = org_rank;
            int add_file = pair.first;
            int add_rank = pair.second;

            while (true) {
                curr_file = curr_file + add_file;
                curr_rank = curr_rank + add_rank;
                /*int check_file = static_cast<int>(curr_file);
                int check_rank = static_cast<int>(curr_file);*/
                if (curr_file == File::OUTOFBOUNDS || curr_rank == Rank::OUTOFBOUNDS) {
                    break;
                }
                bishop_moves.emplace_back(MoveBuilder::basic_move(p, Position(curr_file, curr_rank)));
                bishop_moves.emplace_back(MoveBuilder::basic_capture(p, Position(curr_file, curr_rank)));
            }
        }

        return bishop_moves;
    }

    std::list<Move> PieceMoveGenerator::Knight(const Piece &p) {
        std::list<Move> knight_moves;

        File org_file = p.position_.file_get();
        Rank org_rank = p.position_.rank_get();


        for (auto pair : knight_directions) {
            knight_moves.emplace_back(
                    MoveBuilder::basic_move(p, Position(org_file + pair.first, org_rank + pair.second)));
            knight_moves.emplace_back(
                    MoveBuilder::basic_capture(p, Position(org_file + pair.first, org_rank + pair.second)));
        }

        return knight_moves;
    }

    std::list<Move> PieceMoveGenerator::Rook(const Piece &p) {
        std::list<Move> rook_moves;

        File org_file = p.position_.file_get();
        Rank org_rank = p.position_.rank_get();
        File curr_file = org_file;
        Rank curr_rank = org_rank;

        for (auto pair : rook_directions) {
            int add_file = pair.first;
            int add_rank = pair.second;

            curr_file = org_file;
            curr_rank = org_rank;

            while (true) {
                curr_file = curr_file + add_file;
                curr_rank = curr_rank + add_rank;
                /*int check_file = static_cast<int>(curr_file);
                int check_rank = static_cast<int>(curr_file);*/
                if (curr_file == File::OUTOFBOUNDS || curr_rank == Rank::OUTOFBOUNDS) {
                    break;
                }
                rook_moves.emplace_back(MoveBuilder::basic_move(p, Position(curr_file, curr_rank)));
                rook_moves.emplace_back(MoveBuilder::basic_capture(p, Position(curr_file, curr_rank)));
            }
        }

        return rook_moves;
    }

    std::list<Move> PieceMoveGenerator::Queen(const Piece &p) {
        std::list<Move> queen_moves;

        File org_file = p.position_.file_get();
        Rank org_rank = p.position_.rank_get();
        File curr_file = org_file;
        Rank curr_rank = org_rank;


        for (auto pair : queen_directions) {
            int add_file = pair.first;
            int add_rank = pair.second;

            curr_file = org_file;
            curr_rank = org_rank;

            while (true) {
                curr_file = curr_file + add_file;
                curr_rank = curr_rank + add_rank;
                /*int check_file = static_cast<int>(curr_file);
                int check_rank = static_cast<int>(curr_file);*/
                if (curr_file == File::OUTOFBOUNDS || curr_rank == Rank::OUTOFBOUNDS) {
                    break;
                }
                queen_moves.emplace_back(MoveBuilder::basic_move(p, Position(curr_file, curr_rank)));
                queen_moves.emplace_back(MoveBuilder::basic_capture(p, Position(curr_file, curr_rank)));
            }
        }

        return queen_moves;
    }

    std::list<Move> PieceMoveGenerator::King(const Piece &p) {
        std::list<Move> king_moves;
        if (!p.has_already_moved_) {
            king_moves.emplace_back(MoveBuilder::king_castling(p));
            king_moves.emplace_back(MoveBuilder::queen_castling(p));
        }

        File org_file = p.position_.file_get();
        Rank org_rank = p.position_.rank_get();

        king_moves.emplace_back(MoveBuilder::basic_move(p, Position(org_file - 1, org_rank)));
        king_moves.emplace_back(MoveBuilder::basic_capture(p, Position(org_file - 1, org_rank)));
        king_moves.emplace_back(MoveBuilder::basic_move(p, Position(org_file - 1, org_rank + 1)));
        king_moves.emplace_back(MoveBuilder::basic_capture(p, Position(org_file - 1, org_rank + 1)));
        king_moves.emplace_back(MoveBuilder::basic_move(p, Position(org_file - 1, org_rank - 1)));
        king_moves.emplace_back(MoveBuilder::basic_capture(p, Position(org_file - 1, org_rank - 1)));
        king_moves.emplace_back(MoveBuilder::basic_move(p, Position(org_file, org_rank + 1)));
        king_moves.emplace_back(MoveBuilder::basic_capture(p, Position(org_file, org_rank + 1)));
        king_moves.emplace_back(MoveBuilder::basic_move(p, Position(org_file, org_rank - 1)));
        king_moves.emplace_back(MoveBuilder::basic_capture(p, Position(org_file, org_rank - 1)));
        king_moves.emplace_back(MoveBuilder::basic_move(p, Position(org_file + 1, org_rank)));
        king_moves.emplace_back(MoveBuilder::basic_capture(p, Position(org_file + 1, org_rank)));
        king_moves.emplace_back(MoveBuilder::basic_move(p, Position(org_file + 1, org_rank + 1)));
        king_moves.emplace_back(MoveBuilder::basic_capture(p, Position(org_file + 1, org_rank + 1)));
        king_moves.emplace_back(MoveBuilder::basic_move(p, Position(org_file + 1, org_rank - 1)));
        king_moves.emplace_back(MoveBuilder::basic_capture(p, Position(org_file + 1, org_rank - 1)));

        return king_moves;
    }

}
