#include <err.h>
#include "chessboard.hh"
#include "piece-type.hh"

namespace board {

    void ChessboardImpl::do_move(Move move) {
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
        is_white_turn_ = color == Color::WHITE;
    }

    bool ChessboardImpl::is_move_legal_QUEEN(Move move) {
        if (is_move_legal_BISHOP(move))
            return true;
        return is_move_legal_ROOK(move);
    }

    bool ChessboardImpl::is_move_legal_KING(Move move) {
        auto startFile = move.start_position_.file_get();
        auto startRank = move.start_position_.rank_get();
        auto endFile = move.end_position_.file_get();
        auto endRank = move.end_position_.rank_get();

        if (startFile - 1 == endFile && startRank - 1 == endRank)
            return true;
        if (startFile == endFile && startRank - 1 == endRank)
            return true;
        if (startFile + 1 == endFile && startRank - 1 == endRank)
            return true;

        if (startFile - 1 == endFile && startRank == endRank)
            return true;
        if (startFile + 1 == endFile && startRank == endRank)
            return true;

        if (startFile - 1 == endFile && startRank + 1 == endRank)
            return true;
        if (startFile == endFile && startRank + 1 == endRank)
            return true;
        if (startFile + 1 == endFile && startRank + 1 == endRank)
            return true;
        return false;

        //Need to figure outillegal moves, (cannot put yourself in check)

    }

    bool ChessboardImpl::is_move_legal_KNIGHT(Move move){
        auto startFile = move.start_position_.file_get();
        auto startRank = move.start_position_.rank_get();
        auto endFile = move.end_position_.file_get();
        auto endRank = move.end_position_.rank_get();

        if (startFile  - 1 == endFile && startRank - 2 == endRank)
            return true;
        if (startFile  - 2 == endFile && startRank - 1 == endRank)
            return true;
        if (startFile  - 2 == endFile && startRank + 1 == endRank)
            return true;
        if (startFile  - 1 == endFile && startRank  + 2 == endRank)
            return true;
        if (startFile  + 2 == endFile && startRank - 1 == endRank)
            return true;
        if (startFile  + 1 == endFile && startRank - 2 == endRank)
            return true;
        if (startFile  + 2 == endFile && startRank + 1 == endRank)
            return true;

        return startFile + 1 == endFile && startRank + 2 == endRank;
    }

    bool ChessboardImpl::is_move_legal_BISHOP(Move move) {
        int Fi = 1;
        int Ri = 1;
        if (move.start_position_.file_get() < move.end_position_.file_get() && move.start_position_.rank_get() < move.end_position_.rank_get())
        {
            while (move.start_position_.file_get() + Fi != move.end_position_.file_get() &&
                move.start_position_.rank_get() + Ri != move.end_position_.rank_get())
            {
                if (board_((int)move.start_position_.file_get() + Fi, (int)move.start_position_.rank_get() + Ri).has_value())
                    return false;
                Fi += 1;
                Ri += 1;
            }
            return true;
        }

        else if (move.start_position_.file_get() < move.end_position_.file_get() && move.start_position_.rank_get() > move.end_position_.rank_get())
        {
            while (move.start_position_.file_get() + Fi != move.end_position_.file_get() &&
                   move.start_position_.rank_get() - Ri != move.end_position_.rank_get())
            {
                if (board_((int)move.start_position_.file_get() + Fi, (int)move.start_position_.rank_get() - Ri).has_value())
                    return false;
                Fi += 1;
                Ri += 1;
            }
            return true;
        }

        else if (move.start_position_.file_get() > move.end_position_.file_get() && move.start_position_.rank_get() < move.end_position_.rank_get())
        {
            while (move.start_position_.file_get() - Fi != move.end_position_.file_get() &&
                   move.start_position_.rank_get() + Ri != move.end_position_.rank_get())
            {
                if (board_((int)move.start_position_.file_get() - Fi, (int)move.start_position_.rank_get() + Ri).has_value())
                    return false;
                Fi += 1;
                Ri += 1;
            }
            return true;
        }

        else
        {

            while (move.start_position_.file_get() - Fi != move.end_position_.file_get() &&
                   move.start_position_.rank_get() - Ri != move.end_position_.rank_get())
            {
                if (board_((int)move.start_position_.file_get() - Fi, (int)move.start_position_.rank_get() - Ri).has_value())
                    return false;
                Fi += 1;
                Ri += 1;
            }
            return true;
        }
    }

    bool ChessboardImpl::is_move_legal_PAWN(Move move) {
        if (is_white_turn_)
        {
            if (move.start_position_.rank_get() + 1 != move.end_position_.rank_get())
            {
                 if (move.start_position_.rank_get() == Rank::TWO && move.start_position_.rank_get() + 2 == move.end_position_.rank_get()) {
                     if (board_((int)move.end_position_.file_get(), (int)move.end_position_.rank_get() - 1).has_value())
                         return false;
                     move.is_double_pawn_push_ = false;
                     return true;
                 }
                 if (move.is_capture_)
                 {
                     if (move.start_position_.rank_get() - 1 == move.end_position_.rank_get() && move.start_position_.file_get() - 1 == move.start_position_.file_get())
                         return true;

                     if (move.start_position_.rank_get() + 1 == move.end_position_.rank_get() && move.start_position_.file_get() - 1 == move.start_position_.file_get())
                         return true;
                 }
                 return false;
            }
            return true;
        } else
        {
            if (move.start_position_.rank_get() - 1 != move.end_position_.rank_get())
            {
                if (move.start_position_.rank_get() == Rank::SEVEN && move.start_position_.rank_get() - 2 == move.end_position_.rank_get())
                {
                    if (board_((int)move.end_position_.file_get() - 1, (int)move.end_position_.rank_get()).has_value())
                        return false;
                    move.is_double_pawn_push_ = false;
                    return true;
                }
                if (move.is_capture_)
                {
                    if (move.start_position_.rank_get() + 1 == move.end_position_.rank_get() && move.start_position_.file_get() - 1 == move.start_position_.file_get())
                        return true;

                    if (move.start_position_.rank_get() + 1 == move.end_position_.rank_get() && move.start_position_.file_get() + 1 == move.start_position_.file_get())
                        return true;
                }
                return false;
            }
            return true;
        }
    }

    bool ChessboardImpl::is_move_legal_ROOK(Move move) {
        int Fi = 1;
        int Ri = 1;
        if (move.start_position_.file_get() < move.end_position_.file_get() && move.start_position_.rank_get() == move.end_position_.rank_get())
        {

            while (move.start_position_.file_get() + Fi != move.end_position_.file_get())
            {
                if (board_((int)move.start_position_.file_get() + Fi, (int)move.start_position_.rank_get()).has_value())
                    return false;
                Fi += 1;
            }
            return true;
        }

        else if (move.start_position_.file_get() > move.end_position_.file_get() && move.start_position_.rank_get() == move.end_position_.rank_get())
        {
            while (move.start_position_.file_get() - Fi != move.end_position_.file_get())
            {
                if (board_((int)move.start_position_.file_get() - Fi, (int)move.start_position_.rank_get()).has_value())
                    return false;
                Fi += 1;
            }
            return true;
        }

        else if (move.start_position_.file_get() == move.end_position_.file_get() && move.start_position_.rank_get() < move.end_position_.rank_get())
        {
            while (move.start_position_.rank_get() + Ri != move.end_position_.rank_get())
            {
                if (board_((int)move.start_position_.file_get(), (int)move.start_position_.rank_get() + Ri).has_value())
                    return false;
                Ri += 1;
            }
            return true;
        }

        else
        {
            while (move.start_position_.rank_get() - Ri != move.end_position_.rank_get())
            {
                if (board_((int)move.start_position_.file_get(), (int)move.start_position_.rank_get() - Ri).has_value())
                    return false;
                Ri += 1;
            }
            return true;
        }
    }

    bool ChessboardImpl::is_move_legal(Move move) {

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
                    return is_move_legal_QUEEN(move);
                case PieceType::KING:
                    return is_move_legal_KING(move);
                case PieceType::PAWN:
                    return is_move_legal_PAWN(move);
                case PieceType::KNIGHT:
                    return is_move_legal_KNIGHT(move);
                case PieceType::ROOK:
                    return is_move_legal_ROOK(move);
                case PieceType::BISHOP:
                    return is_move_legal_BISHOP(move);
                default:
                    errx(2, "pieceType ?");
        }
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
        allMoves.remove_if([this](Move m){return !this->is_move_legal(m); });
    
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

    std::optional<Piece> ChessboardImpl::operator[](Position pos) {
        return board_((int)pos.file_get(), (int)pos.rank_get());
    }

    std::string ChessboardImpl::to_string() {
        throw "not implemented";
    }
}
