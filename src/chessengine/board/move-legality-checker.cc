#include "chessboard.hh"

namespace board {

    bool MoveLegalityChecker::is_move_legal_QUEEN(const Chessboard& chessboard, const Move& move) {

        bool b = is_move_legal_BISHOP(chessboard, move);
        if (b)
            return true;
        if (move.start_position_.rank_get() != move.end_position_.rank_get() && move.start_position_.file_get() != move.end_position_.file_get())
            return false;
        return is_move_legal_ROOK(chessboard, move);
    }

    //TODO: use chessboard to assess legality (currently not using it)
    bool MoveLegalityChecker::is_move_legal_KING(const Chessboard& chessboard, const Move& move) {
        auto king = chessboard.board_((int)move.start_position_.file_get(), (int)move.start_position_.rank_get());
        if (move.is_queen_castling_) {
            if (king.value().has_already_moved_)
                return false;
            if (chessboard.is_white_turn_) {
                auto rook = chessboard.board_((int) File::A, (int) Rank::ONE);
                if (!rook.has_value() || rook.value().has_already_moved_)
                    return false;

                if (chessboard.board_((int) File::B, (int) Rank::ONE).has_value() ||
                    chessboard.board_((int) File::C, (int) Rank::ONE).has_value())
                    return false;
            } else {
                auto rook = chessboard.board_((int) File::A, (int) Rank::EIGHT);
                if (!rook.has_value() || rook.value().has_already_moved_)
                    return false;

                if (chessboard.board_((int) File::B, (int) Rank::EIGHT).has_value() ||
                    chessboard.board_((int) File::C, (int) Rank::EIGHT).has_value())
                    return false;
            }
            return true;
        }

        if (move.is_king_castling_)
        {
            if (king.value().has_already_moved_)
                return false;
            if (chessboard.is_white_turn_)
            {
               auto rook = chessboard.board_((int)File::H, (int)Rank::ONE);
               if (!rook.has_value() || rook.value().has_already_moved_)
                   return false;

               if (chessboard.board_((int)File::F, (int)Rank::ONE).has_value() || chessboard.board_((int)File::G, (int)Rank::ONE).has_value())
                   return false;
            } else
            {
                auto rook = chessboard.board_((int)File::H, (int)Rank::EIGHT);
                if (!rook.has_value() || rook.value().has_already_moved_)
                    return false;

                if (chessboard.board_((int)File::F, (int)Rank::EIGHT).has_value() || chessboard.board_((int)File::G, (int)Rank::EIGHT).has_value())
                    return false;
            }
            return true;
        }
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

    //TODO: use chessboard to assess legality (currently not using it)
    bool MoveLegalityChecker::is_move_legal_KNIGHT(const Chessboard&, const Move& move){
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

    bool MoveLegalityChecker::is_move_legal_BISHOP(const Chessboard& b, const Move& move) {
        int Fi = 1;
        int Ri = 1;
        bool bo = true;
        if (move.start_position_.file_get() < move.end_position_.file_get() && move.start_position_.rank_get() < move.end_position_.rank_get())
        {
            while (true)
            {
                if (move.start_position_.file_get() + Fi == File::OUTOFBOUNDS || move.start_position_.rank_get() + Ri == Rank::OUTOFBOUNDS)
                {
                    bo = false;
                    break;
                }
                if (move.start_position_.file_get() + Fi == move.end_position_.file_get() &&
                    move.start_position_.rank_get() + Ri == move.end_position_.rank_get())
                {return true;}
                if (b.board_((int)move.start_position_.file_get() + Fi, (int)move.start_position_.rank_get() + Ri).has_value())
                    return false;
                Fi += 1;
                Ri += 1;

            }
        }
        else if (move.start_position_.file_get() < move.end_position_.file_get() && move.start_position_.rank_get() > move.end_position_.rank_get())
        {
            while (true)
            {
                if (move.start_position_.file_get() + Fi == move.end_position_.file_get() &&
                    move.start_position_.rank_get() - Ri == move.end_position_.rank_get())
                {return true;}
                if (move.start_position_.file_get() + Fi == File::OUTOFBOUNDS || move.start_position_.rank_get() - Ri == Rank::OUTOFBOUNDS)
                {
                    bo = false;
                    break;
                }
                if (b.board_((int)move.start_position_.file_get() + Fi, (int)move.start_position_.rank_get() - Ri).has_value())
                    return false;
                Fi += 1;
                Ri += 1;
            }
        }

        else if (move.start_position_.file_get() > move.end_position_.file_get() && move.start_position_.rank_get() < move.end_position_.rank_get())
        {
            while (true)
            {
                if (move.start_position_.file_get() - Fi == move.end_position_.file_get() &&
                    move.start_position_.rank_get() + Ri == move.end_position_.rank_get())
                {return true;}
                if (move.start_position_.file_get() - Fi == File::OUTOFBOUNDS || move.start_position_.rank_get() + Ri == Rank::OUTOFBOUNDS)
                {
                    bo = false;
                    break;
                }
                if (b.board_((int)move.start_position_.file_get() - Fi, (int)move.start_position_.rank_get() + Ri).has_value())
                    return false;
                Fi += 1;
                Ri += 1;
            }
        }
        else
        {

            while (true)
            {
                if (move.start_position_.file_get() - Fi == move.end_position_.file_get() &&
                    move.start_position_.rank_get() - Ri == move.end_position_.rank_get())
                {return true;}
                if (move.start_position_.file_get() - Fi == File::OUTOFBOUNDS || move.start_position_.rank_get() - Ri == Rank::OUTOFBOUNDS)
                {
                    bo = false;
                    break;
                }
                if (b.board_((int)move.start_position_.file_get() - Fi, (int)move.start_position_.rank_get() - Ri).has_value())
                    return false;
                Fi += 1;
                Ri += 1;
            }
        }
        return bo;
    }

    bool MoveLegalityChecker::is_move_legal_PAWN(const Chessboard& b, const Move& move) {

        //TODO: the position hold in b.en_passant_target_square_ (if any) should be a valid capture position, even if it is empty. In this case, the captured piece
        //is not on move.end_position_ so we will have to handle this case.

        int rank_direction = b.is_white_turn_ ? 1 : -1;
        Rank initial_rank = b.is_white_turn_ ? Rank::TWO : Rank::SEVEN;
        if (move.start_position_.rank_get() + rank_direction != move.end_position_.rank_get())
        {
                if (move.start_position_.rank_get() == initial_rank && move.start_position_.rank_get() + rank_direction * 2 == move.end_position_.rank_get()) {
                    if (b.board_((int)move.end_position_.file_get(), (int)move.end_position_.rank_get() - rank_direction).has_value())
                        return false;
                    return true;
                }
                if (move.is_capture_)
                {
                    if (b.read(move.end_position_) == std::nullopt) {
                        if (!move.is_en_passant_) {
                            return false;
                        }
                        return move.end_position_ == b.en_passant_target_square_;
                    }
                    if (move.start_position_.file_get() - 1 == move.end_position_.file_get() && move.start_position_.rank_get() + rank_direction == move.end_position_.rank_get())
                        return true;

                    if (move.start_position_.file_get() + 1 == move.end_position_.file_get() && move.start_position_.rank_get() + rank_direction == move.end_position_.rank_get())
                        return true;
                }
                return false;
        }
        return true;
    }

    bool MoveLegalityChecker::is_move_legal_ROOK(const Chessboard& b, const Move& move) {
        int Fi = 1;
        int Ri = 1;
        if (move.start_position_.file_get() < move.end_position_.file_get() && move.start_position_.rank_get() == move.end_position_.rank_get())
        {
            while (move.start_position_.file_get() + Fi != move.end_position_.file_get())
            {
                if (move.start_position_.file_get() + Fi == File::OUTOFBOUNDS)
                {
                    return false;
                }
                if (b.board_((int)move.start_position_.file_get() + Fi, (int)move.start_position_.rank_get()).has_value())
                    return false;
                Fi += 1;
            }
            return true;
        }

        else if (move.start_position_.file_get() > move.end_position_.file_get() && move.start_position_.rank_get() == move.end_position_.rank_get())
        {
            while (move.start_position_.file_get() - Fi != move.end_position_.file_get())
            {
                if (move.start_position_.file_get() - Fi == File::OUTOFBOUNDS)
                {
                    return false;
                }
                if (b.board_((int)move.start_position_.file_get() - Fi, (int)move.start_position_.rank_get()).has_value())
                    return false;
                Fi += 1;
            }
            return true;
        }

        else if (move.start_position_.file_get() == move.end_position_.file_get() && move.start_position_.rank_get() < move.end_position_.rank_get())
        {
            while (move.start_position_.rank_get() + Ri != move.end_position_.rank_get())
            {
                if (move.start_position_.rank_get() + Ri == Rank::OUTOFBOUNDS)
                {
                    return false;
                }
                if (b.board_((int)move.start_position_.file_get(), (int)move.start_position_.rank_get() + Ri).has_value())
                    return false;
                Ri += 1;
            }
            return true;
        }

        else
        {
            while (move.start_position_.rank_get() - Ri != move.end_position_.rank_get())
            {
                if (move.start_position_.rank_get() - Ri == Rank::OUTOFBOUNDS)
                {
                    return false;
                }
                if (b.board_((int)move.start_position_.file_get(), (int)move.start_position_.rank_get() - Ri).has_value())
                    return false;
                Ri += 1;
            }
            return true;
        }
    }

    bool MoveLegalityChecker::is_move_legal(const Chessboard& b, const Move &move) {
        switch (move.piece_) {
            case PieceType::PAWN:
                return is_move_legal_PAWN(b, move);
            case PieceType::BISHOP:
                return is_move_legal_BISHOP(b, move);
            case PieceType::KNIGHT:
                return is_move_legal_KNIGHT(b, move);
            case PieceType::ROOK:
                return is_move_legal_ROOK(b, move);
            case PieceType::QUEEN:
                return is_move_legal_QUEEN(b, move);
            case PieceType::KING:
                return is_move_legal_KING(b, move);
            default:
                return false;
        }
    }

}
