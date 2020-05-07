#include <err.h>
#include "chessboard.hh"
#include "piece-type.hh"

namespace board {

    void ChessboardImpl::do_move(Move) {
        throw "not implemented";
    }


    bool ChessboardImpl::is_move_legal_QUEEN(Move move) {
        if (is_move_legal_BISHOP(move))
            return true;
        return is_move_legal_ROOK(move);
    }

    bool ChessboardImpl::is_move_legal_KING(Move move) {
        if (is_move_legal_BISHOP(move))
            return true;
        return is_move_legal_ROOK(move);
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
                 if (move.is_double_pawn_push_ && move.start_position_.rank_get() + 2 == move.end_position_.rank_get()) {
                     if (board_((int)move.end_position_.file_get(), (int)move.end_position_.rank_get() - 1).has_value())
                         return false;
                     move.is_double_pawn_push_ = false;
                     return true;
                 }
                 return false;
            }
            return true;
        } else
        {
            if (move.start_position_.rank_get() - 1 != move.end_position_.rank_get())
            {
                if (move.is_double_pawn_push_ && move.start_position_.rank_get() - 2 == move.end_position_.rank_get())
                {
                    if (board_((int)move.end_position_.file_get() - 1, (int)move.end_position_.rank_get()).has_value())
                        return false;
                    move.is_double_pawn_push_ = false;
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
        auto piece = board_((int)move.end_position_.file_get(), (int)move.end_position_.rank_get());
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
