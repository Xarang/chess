#include "piece.hh"

namespace board {

    class PieceMoveGenerator {
        public:
        static std::list<Move> Pawn(Piece& p) {
            std::list<Move> moves;
            int direction = p.color_ == Color::WHITE ? 1 : -1;
            //double pawn advance
            if (!p.has_already_moved_) {
                moves.push_front(Move::DoublePawnPush(p));
            }
            //move forward
            moves.push_front(Move::BasicMove(p, Position(p.position_.file_get(), p.position_.rank_get() + direction)));
            //capture in diagonals
            moves.push_front(Move::BasicCapture(p, Position(p.position_.file_get() + 1, p.position_.rank_get() + direction)));
            moves.push_front(Move::BasicCapture(p, Position(p.position_.file_get() - 1, p.position_.rank_get() + direction)));
            return moves;
        }

        static std::list<Move> Bishop(Piece&) {
            throw "not implemented";
        }

        static std::list<Move> Knight(Piece&) {
            throw "not implemented";
        }

        static std::list<Move> Rook(Piece&) {
            throw "not implemented";
        }

        static std::list<Move> Queen(Piece&) {
            throw "not implemented";
        }

        static std::list<Move> King(Piece& p) {
            if (!p.has_already_moved_ == false) {
                //castling
            }
            throw "not implemented";
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