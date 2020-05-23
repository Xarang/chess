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

    static const std::pair<int,int> king_directions[] {
            {0,-1},
            {0,1},
            {1,0},
            {-1,0},
            {1,-1},
            {-1,1},
            {-1,-1},
            {1,1},
    };




    void PieceMoveGenerator::Pawn(const Piece &p, std::list<Move>& m) {
        //std::list<Move> moves;
        int direction = p.color_ == Color::WHITE ? 1 : -1;
        //double pawn advance
        if (!p.has_already_moved_) {
            m.emplace_front(Move());
            MoveBuilder::set_double_pawn_push(m.front(), p);
        }
        //move forward
        m.emplace_front(Move());
        MoveBuilder::set_basic_move(m.front(), p, Position(p.position_.file_get(), p.position_.rank_get() + direction));
        //capture in diagonals
        m.emplace_front(Move());
        MoveBuilder::set_basic_capture(m.front(), p, Position(p.position_.file_get() + 1,
                                                                p.position_.rank_get() + direction));
        m.emplace_front(Move());
        MoveBuilder::set_basic_capture(m.front(), p, Position(p.position_.file_get() - 1,
                                                                p.position_.rank_get() + direction));

        for (Move& move : m) {
            if (move.piece_ == PieceType::PAWN &&
                    ((move.end_position_.rank_get() == Rank::EIGHT && p.color_ == Color::WHITE)
                    || (move.end_position_.rank_get() == Rank::ONE && p.color_ == Color::BLACK))) {
                m.emplace_front(Move());
                MoveBuilder::set_with_promotion(m.front(), move, PieceType::BISHOP);
                m.emplace_front(Move());
                MoveBuilder::set_with_promotion(m.front(), move, PieceType::QUEEN);
                m.emplace_front(Move());
                MoveBuilder::set_with_promotion(m.front(), move, PieceType::ROOK);
                m.emplace_front(Move());
                MoveBuilder::set_with_promotion(m.front(), move, PieceType::KNIGHT);
            }
        }

        //std::cout << "generated potential moves for piece: " << p.to_string() << "\n";
       
    }

    void PieceMoveGenerator::Bishop(const Piece &p, std::list<Move>& m) {
        //std::list<Move> bishop_moves;
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
                m.emplace_back(Move());
                MoveBuilder::set_basic_move(m.back(), p, Position(curr_file, curr_rank));
                m.emplace_back(Move());
                MoveBuilder::set_basic_capture(m.back(), p, Position(curr_file, curr_rank));
            }
        }
    }

    void PieceMoveGenerator::Knight(const Piece &p, std::list<Move>& m) {
        //std::list<Move> knight_moves;

        File org_file = p.position_.file_get();
        Rank org_rank = p.position_.rank_get();


        for (auto pair : knight_directions) {
            m.emplace_back(Move());
            MoveBuilder::set_basic_move(m.back(), p, Position(org_file + pair.first, org_rank + pair.second));
            m.emplace_back(Move());
            MoveBuilder::set_basic_capture(m.back(), p, Position(org_file + pair.first, org_rank + pair.second));
        }
    }

    void PieceMoveGenerator::Rook(const Piece &p, std::list<Move>& m) {
        //std::list<Move> rook_moves;

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
                m.emplace_back(Move());
                MoveBuilder::set_basic_move(m.back(), p, Position(curr_file, curr_rank));
                m.emplace_back(Move());
                MoveBuilder::set_basic_capture(m.back(), p, Position(curr_file, curr_rank));
            }
        }
    }

    void PieceMoveGenerator::Queen(const Piece &p, std::list<Move>& m) {
        //std::list<Move> queen_moves;

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
                m.emplace_back(Move());
                MoveBuilder::set_basic_move(m.back(), p, Position(curr_file, curr_rank));
                m.emplace_back(Move());
                MoveBuilder::set_basic_capture(m.back(), p, Position(curr_file, curr_rank));
            }
        }
    }

    void PieceMoveGenerator::King(const Piece &p, std::list<Move>& m) {
        //std::list<Move> king_moves;
        if (!p.has_already_moved_) {
            m.emplace_back(Move());
            MoveBuilder::set_king_castling(m.back(), p);
            m.emplace_back(Move());
            MoveBuilder::set_queen_castling(m.back(), p);
        }

        File org_file = p.position_.file_get();
        Rank org_rank = p.position_.rank_get();
        for (auto pair : king_directions) {
            m.emplace_back(Move());
            MoveBuilder::set_basic_move(m.back(), p, Position(org_file + pair.first, org_rank + pair.second));
            m.emplace_back(Move());
            MoveBuilder::set_basic_capture(m.back(), p, Position(org_file + pair.first, org_rank + pair.second));
        }
    }

}
