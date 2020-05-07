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
                moves.push_front(MoveBuilder::DoublePawnPush(p));
            }
            //move forward
            moves.push_front(MoveBuilder::BasicMove(p, Position(p.position_.file_get(), p.position_.rank_get() + direction)));
            //capture in diagonals
            moves.push_front(MoveBuilder::BasicCapture(p, Position(p.position_.file_get() + 1, p.position_.rank_get() + direction)));
            moves.push_front(MoveBuilder::BasicCapture(p, Position(p.position_.file_get() - 1, p.position_.rank_get() + direction)));
            return moves;
        }

        static std::list<Move> Bishop(Piece& p) {
            std::list<Move> bishop_moves;
            File org_file = p.position_.file_get();
            Rank org_rank = p.position_.rank_get();
            File curr_file = org_file;
            Rank curr_rank = org_rank;

            std::pair<int, int> pairs[4] = {
                std::make_pair(1, 1),
                std::make_pair(-1, -1),
                std::make_pair(1, -1);
                std::make_pair(-1, 1)
            };

            for (int i = 0; i < 4; i++)
            {
                std::pair<int, int> pair = pairs[i];
                int add_file = pair.first;
                int add_rank = pair.second;

                while (true)
                {
                    curr_file = curr_file + add_file;
                    curr_rank = curr_rank + add_rank;
                    int check_file = static_cast<int>(curr_file);
                    int check_rank = static_cast<int>(curr_file);
                    if (check_file == -1 || check_rank == -1)
                    {
                        break;
                    }
                    bishop_moves.push_back(MoveBuilder::BasicMove(p, Position(curr_file, curr_rank)));
                    bishop_moves.push_back(MoveBuilder::BasicCapture(p, Position(curr_file, curr_rank)));
                }
            }

            return bishop_moves;
        }

        static std::list<Move> Knight(Piece& p) {
            std::list<Move> knight_moves;

            File org_file = p.position_.file_get();
            Rank org_rank = p.position_.rank_get();

            knigh_moves.push_back(MoveBuilder::BasicMove(p, Position(org_file + 1, org_rank + 2)));
            knigh_moves.push_back(MoveBuilder::BasicCapture(p, Position(org_file + 1, org_rank + 2)));

            knigh_moves.push_back(MoveBuilder::BasicMove(p, Position(org_file + 1, org_rank - 2)));
            knigh_moves.push_back(MoveBuilder::BasicCapture(p, Position(org_file + 1, org_rank - 2)));

            knigh_moves.push_back(MoveBuilder::BasicMove(p, Position(org_file - 1, org_rank + 2)));
            knigh_moves.push_back(MoveBuilder::BasicCapture(p, Position(org_file - 1, org_rank - 2)));

            knigh_moves.push_back(MoveBuilder::BasicMove(p, Position(org_file + 2, org_rank + 1)));
            knigh_moves.push_back(MoveBuilder::BasicCapture(p, Position(org_file + 2, org_rank + 1)));

            knigh_moves.push_back(MoveBuilder::BasicMove(p, Position(org_file + 1, org_rank + 2)));
            knigh_moves.push_back(MoveBuilder::BasicCapture(p, Position(org_file + 1, org_rank + 2)));

            knigh_moves.push_back(MoveBuilder::BasicMove(p, Position(org_file + 1, org_rank + 2)));
            knigh_moves.push_back(MoveBuilder::BasicCapture(p, Position(org_file + 1, org_rank + 2)));

            knigh_moves.push_back(MoveBuilder::BasicMove(p, Position(org_file + 1, org_rank + 2)));
            knigh_moves.push_back(MoveBuilder::BasicCapture(p, Position(org_file + 1, org_rank + 2)));

            knigh_moves.push_back(MoveBuilder::BasicMove(p, Position(org_file + 1, org_rank + 2)));
            knigh_moves.push_back(MoveBuilder::BasicCapture(p, Position(org_file + 1, org_rank + 2)));

            return knight_moves;
        }

        static std::list<Move> Rook(Piece&) {
            return std::list<Move>();
        }

        static std::list<Move> Queen(Piece&) {
            return std::list<Move>();
        }

        static std::list<Move> King(Piece& p)   {
            if (!p.has_already_moved_ == false) {
                //castling
            }
            return std::list<Move>();
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
