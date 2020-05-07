#include "chessboard.hh"

namespace board {

    void ChessboardImpl::do_move(Move) {
        throw "not implemented";
    }

    bool ChessboardImpl::is_move_legal(Move) {
        throw "not implemented";
    }

    std::list<Move> ChessboardImpl::generateLegalMoves() {
        std::list<Move> allMoves;

        //build the list of all "potential" moves, not accounting for OOB and blocked paths
        for (auto piece : pieces_) {
            if ((bool)piece.color_ != is_white_turn_) {
                std::list<Move> pieceMoves = piece.getAllPotentialMoves();
                for (auto move : pieceMoves) {
                    allMoves.push_front(move);
                }
            }
        }
        
        //TODO: figure out best way to do this
        //allMoves.remove_if((Move move) => is_move_legal(move));
    
        return allMoves;
    }

    bool ChessboardImpl::is_check() {
        throw "not implemented";
    }

    bool ChessboardImpl::is_checkmate() {
        throw "not implemented";
    }

    bool ChessboardImpl::is_draw() {
        throw "not implemented";
    }

    std::optional<Piece> ChessboardImpl::operator[](Position) {
        throw "not implemented";
    }

    std::string ChessboardImpl::to_string() {
        throw "not implemented";
    }
}
