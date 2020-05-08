#include <err.h>
#include "chessboard.hh"
#include "piece-type.hh"

namespace board {

    void Chessboard::do_move(Move move) {
        int j = 0;
        Color color;
        Rank end;

        if (is_white_turn_) {
            color = Color::WHITE;
            end = Rank::EIGHT;
        }
        else {
            color = Color::BLACK;
            end = Rank::ONE;
        }

        for (auto ite = pieces_.begin(); ite < pieces_.end(); ite++)
        {
            if (ite->position_ == move.end_position_)
            {
                pieces_.erase(ite);
            }
        }

        for (unsigned  long i = 0; i < pieces_.size(); i++)
        {
            if (pieces_[i].position_ == move.start_position_)
            {
                pieces_[i].position_ = move.end_position_;
                pieces_[i].has_already_moved_ = true;
                j = i;
                break;
            }
        }

        (*this)[move.end_position_] = std::make_optional(pieces_[j]);
        (*this)[move.start_position_] = std::nullopt;

        if ((*this)[move.end_position_].value().type_ == PieceType::PAWN && move.end_position_.rank_get() == end)
        {
            //Progression to queen ?
            Piece piece(move.end_position_, color, PieceType::QUEEN);
            (*this)[move.end_position_] = piece;
        }
        is_white_turn_ = !is_white_turn_;
    }


    bool Chessboard::is_move_legal(Move move) {

        if (!(*this)[move.start_position_].has_value())
            return false;

        auto piece = (*this)[move.end_position_];
        if (piece.has_value())
        {
            if (piece.value().color_ == Color::WHITE && is_white_turn_)
                return false;
            if (piece.value().color_ == Color::BLACK && !is_white_turn_)
                return false;
        }
        switch (move.piece_) {
                case PieceType::QUEEN:
                    return MoveLegalityChecker::is_move_legal_QUEEN(*this, move);
                case PieceType::KING:
                    return MoveLegalityChecker::is_move_legal_KING(*this, move);
                case PieceType::PAWN:
                    return MoveLegalityChecker::is_move_legal_PAWN(*this, move);
                case PieceType::KNIGHT:
                    return MoveLegalityChecker::is_move_legal_KNIGHT(*this, move);
                case PieceType::ROOK:
                    return MoveLegalityChecker::is_move_legal_ROOK(*this, move);
                case PieceType::BISHOP:
                    return MoveLegalityChecker::is_move_legal_BISHOP(*this, move);
                default:
                    errx(2, "pieceType ?");
        }
      }

    std::list<Move> Chessboard::generateLegalMoves() {
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
        allMoves.remove_if([this](Move m){return !this->is_move_legal(m); });
    
        return allMoves;
    }

    Chessboard Chessboard::project(Move move) {
        auto copy = Chessboard(*this);
        copy.do_move(move);
        return copy;
    }

    bool Chessboard::is_check() {
        throw "not implemented";
    }

    bool Chessboard::is_checkmate() {
        throw "not implemented";
    }

    bool Chessboard::is_draw() {
        throw "not implemented";
    }

    std::optional<Piece>& Chessboard::operator[](Position pos) {
        return board_((int)pos.file_get(), (int)pos.rank_get());
    }

    std::string Chessboard::to_string() {
        throw "not implemented";
    }
}
