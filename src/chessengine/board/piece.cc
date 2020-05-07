#include "piece.hh"

namespace board {


    class MoveGenerator {
        public:
        static std::list<Move> Pawn(Piece p) {

        }

        static std::list<Move> Bishop(Piece& p) {

        }

        static std::list<Move> Knight(Piece& p) {

        }

        static std::list<Move> Rook(Piece& p) {

        }

        static std::list<Move> Queen(Piece& p) {

        }

        static std::list<Move> King(Piece& p) {

        }
    };
   


    std::list<Move> Piece::getAllPotentialMoves() {
        switch (type_) {
            case PieceType::PAWN:
                return MoveGenerator::Pawn(*this);
            case PieceType::BISHOP:
                return MoveGenerator::Bishop(*this);
            case PieceType::KNIGHT:
                return MoveGenerator::Knight(*this);
            case PieceType::ROOK:
                return MoveGenerator::Rook(*this);
            case PieceType::QUEEN:
                return MoveGenerator::Queen(*this);
            case PieceType::KING:
                return MoveGenerator::King(*this);
            default:
                return std::list<Move>();
        }
    }
}