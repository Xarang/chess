#include "chessboard.hh"

namespace board {

    bool MoveLegalityChecker::is_move_legal_QUEEN(Chessboard b, Move move) {
        if (is_move_legal_BISHOP(b, move))
            return true;
        return is_move_legal_ROOK(b, move);
    }

    //TODO: use chessboard to assess legality (currently not using it)
    bool MoveLegalityChecker::is_move_legal_KING(Chessboard, Move move) {
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
    bool MoveLegalityChecker::is_move_legal_KNIGHT(Chessboard, Move move){
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

    bool MoveLegalityChecker::is_move_legal_BISHOP(Chessboard b, Move move) {
        int Fi = 1;
        int Ri = 1;
        if (move.start_position_.file_get() < move.end_position_.file_get() && move.start_position_.rank_get() < move.end_position_.rank_get())
        {
            while (move.start_position_.file_get() + Fi != move.end_position_.file_get() &&
                move.start_position_.rank_get() + Ri != move.end_position_.rank_get())
            {
                if (b.board_((int)move.start_position_.file_get() + Fi, (int)move.start_position_.rank_get() + Ri).has_value())
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
                if (b.board_((int)move.start_position_.file_get() + Fi, (int)move.start_position_.rank_get() - Ri).has_value())
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
                if (b.board_((int)move.start_position_.file_get() - Fi, (int)move.start_position_.rank_get() + Ri).has_value())
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
                if (b.board_((int)move.start_position_.file_get() - Fi, (int)move.start_position_.rank_get() - Ri).has_value())
                    return false;
                Fi += 1;
                Ri += 1;
            }
            return true;
        }
    }

    bool MoveLegalityChecker::is_move_legal_PAWN(Chessboard b, Move move) {
        if (b.is_white_turn_)
        {
            if (move.start_position_.rank_get() + 1 != move.end_position_.rank_get())
            {
                    if (move.start_position_.rank_get() == Rank::TWO && move.start_position_.rank_get() + 2 == move.end_position_.rank_get()) {
                        if (b.board_((int)move.end_position_.file_get(), (int)move.end_position_.rank_get() - 1).has_value())
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
                    if (b.board_((int)move.end_position_.file_get() - 1, (int)move.end_position_.rank_get()).has_value())
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

    bool MoveLegalityChecker::is_move_legal_ROOK(Chessboard b, Move move) {
        int Fi = 1;
        int Ri = 1;
        if (move.start_position_.file_get() < move.end_position_.file_get() && move.start_position_.rank_get() == move.end_position_.rank_get())
        {

            while (move.start_position_.file_get() + Fi != move.end_position_.file_get())
            {
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
                if (b.board_((int)move.start_position_.file_get(), (int)move.start_position_.rank_get() - Ri).has_value())
                    return false;
                Ri += 1;
            }
            return true;
        }
    }

}
