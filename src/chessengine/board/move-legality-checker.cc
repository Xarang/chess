#include "chessboard.hh"

namespace board {

    bool MoveLegalityChecker::is_move_legal_QUEEN(Chessboard chessboard, Move move) {
        if (move.is_queen_castling_)
        {
            auto king = chessboard.board_((int)move.start_position_.file_get(), (int)move.start_position_.rank_get());
            if (king.value().has_already_moved_)
                return false;
            if (chessboard.is_white_turn_)
            {
                auto rook = chessboard.board_(((int)File::H, (int)Rank::ONE));
                if (!rook.has_value() || rook.value().has_already_moved_)
                    return false;

                if (chessboard.board_((int)File::F, (int)Rank::ONE).has_value() || chessboard.board_((int)File::G, (int)Rank::ONE).has_value())
                    return false;
            } else
            {
                auto rook = chessboard.board_(((int)File::H, (int)Rank::EIGHT));
                if (!rook.has_value() || rook.value().has_already_moved_)
                    return false;

                if (chessboard.board_((int)File::F, (int)Rank::EIGHT).has_value() || chessboard.board_((int)File::G, (int)Rank::EIGHT).has_value())
                    return false;
            }
            return true;
        if (is_move_legal_BISHOP(chessboard, move))
            return true;
        return is_move_legal_ROOK(chessboard, move);
    }

    //TODO: use chessboard to assess legality (currently not using it)
    bool MoveLegalityChecker::is_move_legal_KING(Chessboard chessboard, Move move) {

        if (move.is_king_castling_)
        {
            auto king = chessboard.board_((int)move.start_position_.file_get(), (int)move.start_position_.rank_get());
            if (king.value().has_already_moved_)
                return false;
            if (chessboard.is_white_turn_)
            {
               auto rook = chessboard.board_(((int)File::A, (int)Rank::ONE));
               if (!rook.has_value() || rook.value().has_already_moved_)
                   return false;

               if (chessboard.board_((int)File::B, (int)Rank::ONE).has_value() || chessboard.board_((int)File::C, (int)Rank::ONE).has_value())
                   return false;
            } else
            {
                auto rook = chessboard.board_(((int)File::A, (int)Rank::EIGHT));
                if (!rook.has_value() || rook.value().has_already_moved_)
                    return false;

                if (chessboard.board_((int)File::B, (int)Rank::EIGHT).has_value() || chessboard.board_((int)File::C, (int)Rank::EIGHT).has_value())
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

        //TODO: the position hold in b.en_passant_target_square_ (if any) should be a valid capture position, even if it is empty. In this case, the captured piece
        //is not on move.end_position_ so we will have to handle this case.

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
