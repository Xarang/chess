#include <iostream>

#include "piece.hh"
#include "move-builder.hh"

namespace board {

    class PieceMoveGenerator {
        public:
        static std::list<Move> Pawn(Piece& p) {
            std::list<Move> moves;
            int direction = p.color_ == Color::WHITE ? 1 : -1;
            //double pawn advance
            if (!p.has_already_moved_) {
                moves.push_front(MoveBuilder::double_pawn_push(p));
            }
            //move forward
            moves.push_front(MoveBuilder::basic_move(p, Position(p.position_.file_get(), p.position_.rank_get() + direction)));
            //capture in diagonals
            moves.push_front(MoveBuilder::basic_capture(p, Position(p.position_.file_get() + 1, p.position_.rank_get() + direction)));
            moves.push_front(MoveBuilder::basic_capture(p, Position(p.position_.file_get() - 1, p.position_.rank_get() + direction)));

            std::list<Move> promotion_moves;
            for (auto move : moves) {
                if ((move.end_position_.rank_get() == Rank::ONE && p.color_ == Color::WHITE)
                    || (move.end_position_.rank_get() == Rank::EIGHT && p.color_ == Color::BLACK)) {
                    promotion_moves.push_front(MoveBuilder::with_promotion(move, PieceType::BISHOP));
                    promotion_moves.push_front(MoveBuilder::with_promotion(move, PieceType::KNIGHT));
                    promotion_moves.push_front(MoveBuilder::with_promotion(move, PieceType::ROOK));
                    promotion_moves.push_front(MoveBuilder::with_promotion(move, PieceType::QUEEN));
                }
            }
            for (auto move : promotion_moves)
            {
                moves.push_back(move);
            }
            return moves;
        }

        static std::list<Move> Bishop(Piece& p) {
            std::list<Move> bishop_moves;
            File org_file = p.position_.file_get();
            Rank org_rank = p.position_.rank_get();


            std::pair<int, int> pairs[4] = {
                std::make_pair(1, 1),
                std::make_pair(-1, -1),
                std::make_pair(1, -1),
                std::make_pair(-1, 1)
            };

            for (int i = 0; i < 4; i++)
            {
                std::pair<int, int> pair = pairs[i];
                
                File curr_file = org_file;
                Rank curr_rank = org_rank;
                int add_file = pair.first;
                int add_rank = pair.second;

                while (true)
                {
                    curr_file = curr_file + add_file;
                    curr_rank = curr_rank + add_rank;
                    /*int check_file = static_cast<int>(curr_file);
                    int check_rank = static_cast<int>(curr_file);*/
                    if (curr_file == File::OUTOFBOUNDS || curr_rank == Rank::OUTOFBOUNDS)
                    {
                        break;
                    }
                    bishop_moves.push_back(MoveBuilder::basic_move(p, Position(curr_file, curr_rank)));
                    bishop_moves.push_back(MoveBuilder::basic_capture(p, Position(curr_file, curr_rank)));
                }
            }

            return bishop_moves;
        }

        static std::list<Move> Knight(Piece& p) {
            std::list<Move> knight_moves;

            File org_file = p.position_.file_get();
            Rank org_rank = p.position_.rank_get();
            
            std::pair<int, int> pairs[] = {
                std::make_pair(1, 2),
                std::make_pair(1, -2),
                std::make_pair(-1, 2),
                std::make_pair(-1, -2),
                std::make_pair(2, 1),
                std::make_pair(2, -1),
                std::make_pair(-2, 1),
                std::make_pair(-2, -1)
            };


            for (auto pair : pairs) {
                knight_moves.push_back(MoveBuilder::basic_move(p, Position(org_file + pair.first, org_rank + pair.second)));
                knight_moves.push_back(MoveBuilder::basic_capture(p, Position(org_file + pair.first, org_rank + pair.second)));
            }

            return knight_moves;
        }

        static std::list<Move> Rook(Piece& p) {
            std::list<Move> rook_moves;

            File org_file = p.position_.file_get();
            Rank org_rank = p.position_.rank_get();
            File curr_file = org_file;
            Rank curr_rank = org_rank;
            std::pair<int, int> pairs[4] = {
                std::make_pair(1, 0),
                std::make_pair(-1, 0),
                std::make_pair(0, 1),
                std::make_pair(0, -1)
            };

            for (int i = 0; i < 4; i++)
            {
                std::pair<int, int> pair = pairs[i];
                int add_file = pair.first;
                int add_rank = pair.second;

                curr_file = org_file;
                curr_rank = org_rank;

                while (true)
                {
                    curr_file = curr_file + add_file;
                    curr_rank = curr_rank + add_rank;
                    /*int check_file = static_cast<int>(curr_file);
                    int check_rank = static_cast<int>(curr_file);*/
                    if (curr_file == File::OUTOFBOUNDS || curr_rank == Rank::OUTOFBOUNDS)
                    {
                        break;
                    }
                    rook_moves.push_back(MoveBuilder::basic_move(p, Position(curr_file, curr_rank)));
                    rook_moves.push_back(MoveBuilder::basic_capture(p, Position(curr_file, curr_rank)));
                }
            }

            return rook_moves;
        }

        static std::list<Move> Queen(Piece& p) {
            std::list<Move> queen_moves;

            File org_file = p.position_.file_get();
            Rank org_rank = p.position_.rank_get();
            File curr_file = org_file;
            Rank curr_rank = org_rank;

            std::pair<int, int> pairs[8] = {
                std::make_pair(1, 1),
                std::make_pair(-1, -1),
                std::make_pair(1, -1),
                std::make_pair(-1, 1),
                std::make_pair(1, 0),
                std::make_pair(-1, 0),
                std::make_pair(0, 1),
                std::make_pair(0, -1)
            };

            for (int i = 0; i < 8; i++)
            {
                std::pair<int, int> pair = pairs[i];
                int add_file = pair.first;
                int add_rank = pair.second;

                curr_file = org_file;
                curr_rank = org_rank;

                while (true)
                {
                    curr_file = curr_file + add_file;
                    curr_rank = curr_rank + add_rank;
                    /*int check_file = static_cast<int>(curr_file);
                    int check_rank = static_cast<int>(curr_file);*/
                    if (curr_file == File::OUTOFBOUNDS || curr_rank == Rank::OUTOFBOUNDS)
                    {
                        break;
                    }
                    queen_moves.push_back(MoveBuilder::basic_move(p, Position(curr_file, curr_rank)));
                    queen_moves.push_back(MoveBuilder::basic_capture(p, Position(curr_file, curr_rank)));
                }
            }

            return queen_moves;
        }

        static std::list<Move> King(Piece& p)   {
            std::list<Move> king_moves;
            if (!p.has_already_moved_) {
                king_moves.push_back(MoveBuilder::king_castling(p));
                king_moves.push_back(MoveBuilder::queen_castling(p));
            }

            File org_file = p.position_.file_get();
            Rank org_rank = p.position_.rank_get();

            king_moves.push_back(MoveBuilder::basic_move(p, Position(org_file - 1, org_rank)));
            king_moves.push_back(MoveBuilder::basic_capture(p, Position(org_file - 1, org_rank)));

            king_moves.push_back(MoveBuilder::basic_move(p, Position(org_file - 1, org_rank + 1)));
            king_moves.push_back(MoveBuilder::basic_capture(p, Position(org_file - 1, org_rank + 1)));

            king_moves.push_back(MoveBuilder::basic_move(p, Position(org_file - 1, org_rank - 1)));
            king_moves.push_back(MoveBuilder::basic_capture(p, Position(org_file - 1, org_rank - 1)));

            king_moves.push_back(MoveBuilder::basic_move(p, Position(org_file, org_rank + 1)));
            king_moves.push_back(MoveBuilder::basic_capture(p, Position(org_file, org_rank + 1)));

            king_moves.push_back(MoveBuilder::basic_move(p, Position(org_file, org_rank - 1)));
            king_moves.push_back(MoveBuilder::basic_capture(p, Position(org_file, org_rank - 1)));

            king_moves.push_back(MoveBuilder::basic_move(p, Position(org_file + 1, org_rank)));
            king_moves.push_back(MoveBuilder::basic_capture(p, Position(org_file + 1, org_rank)));

            king_moves.push_back(MoveBuilder::basic_move(p, Position(org_file + 1, org_rank + 1)));
            king_moves.push_back(MoveBuilder::basic_capture(p, Position(org_file + 1, org_rank + 1)));

            king_moves.push_back(MoveBuilder::basic_move(p, Position(org_file + 1, org_rank - 1)));
            king_moves.push_back(MoveBuilder::basic_capture(p, Position(org_file + 1, org_rank - 1)));
            
            return king_moves;
        }
    };



    std::list<Move> Piece::getAllPotentialMoves() {
        switch (type_) {
            case PieceType::PAWN:
                return PieceMoveGenerator::Pawn(*this);
            case PieceType::BISHOP:
                return PieceMoveGenerator::Bishop(*this);
            case PieceType::KNIGHT:
                return PieceMoveGenerator::Knight(*this);
            case PieceType::ROOK:
                return PieceMoveGenerator::Rook(*this);
            case PieceType::QUEEN:
                return PieceMoveGenerator::Queen(*this);
            case PieceType::KING:
                return PieceMoveGenerator::King(*this);
            default:
                return std::list<Move>();
        }
    }
}
