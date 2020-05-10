#include "chessboard.hh"

namespace board {

    bool MoveLegalityChecker::is_move_legal_QUEEN(const Chessboard& chessboard, Move& move) {

        bool b = is_move_legal_BISHOP(chessboard, move);
        if (b)
            return true;
        if (move.start_position_.rank_get() != move.end_position_.rank_get() && move.start_position_.file_get() != move.end_position_.file_get())
            return false;
        b = is_move_legal_ROOK(chessboard, move);
        return b;
    }

    //TODO: use chessboard to assess legality (currently not using it)
    bool MoveLegalityChecker::is_move_legal_KING(const Chessboard& chessboard, Move& move) {
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
    bool MoveLegalityChecker::is_move_legal_KNIGHT(const Chessboard&, Move& move){
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

    bool MoveLegalityChecker::is_move_legal_BISHOP(const Chessboard& b, Move& move) {
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
                {
                    return false;
                }

                Fi += 1;
                Ri += 1;
            }
        }
        return bo;
    }

    bool MoveLegalityChecker::is_move_legal_PAWN(const Chessboard& b, Move& move) {

        //TODO: the position hold in b.en_passant_target_square_ (if any) should be a valid capture position, even if it is empty. In this case, the captured piece
        //is not on move.end_position_ so we will have to handle this case.

        //std::cout << "checking legality of pawn move: " + move.to_string() << "\n";
        int rank_direction = b.is_white_turn_ ? 1 : -1;
        Rank initial_rank = b.is_white_turn_ ? Rank::TWO : Rank::SEVEN;

        if (move.is_double_pawn_push_) {
            if (move.start_position_.rank_get() != initial_rank || move.is_capture_) {
                return false;
            }
            return (move.start_position_.rank_get() + rank_direction * 2 == move.end_position_.rank_get() &&
                    move.start_position_.file_get() == move.end_position_.file_get());
        }
        else if (move.is_capture_) {
            if (move.start_position_.rank_get() + rank_direction != move.end_position_.rank_get()) {
                return false;
            }
            if (move.start_position_.file_get() + 1 != move.end_position_.file_get() && move.start_position_.file_get() - 1 != move.end_position_.file_get()) {
                return false;
            }
            if (b.read(move.end_position_) == std::nullopt) {
                if (!move.is_en_passant_) {
                    return false;
                }
                return move.end_position_ == b.en_passant_target_square_;
            }
            else if (move.is_en_passant_) {
                //if the move happens to be a regular capture (on a non-empty square) and was labelled en passant by the pgn parser or move generator,
                //then we know it is not an actual en passant move. we set the flag to false, and relaunch it all the way up in the is_legal_procedure
                //because being "potentially en passant" made us skip some verifications on this move, that we want to do now.
                //std::cout << move.to_string() << " is actually not en passant " << "\n";
                move.is_en_passant_ = false;
                return is_move_legal(b, move);
            }
        }
        else {
            return (move.start_position_.file_get() == move.end_position_.file_get() && move.start_position_.rank_get() + rank_direction == move.end_position_.rank_get());
        }

        /*if (move.start_position_.rank_get() + rank_direction != move.end_position_.rank_get())
        {
                if (move.start_position_.rank_get() == initial_rank && move.start_position_.rank_get() + rank_direction * 2 == move.end_position_.rank_get()) {
                    move.is_double_pawn_push_ = true;
                    if (b.board_((int)move.end_position_.file_get(), (int)move.end_position_.rank_get() - rank_direction).has_value())
                        return false;
                    return true;
                }
                if (move.is_capture_)
                {
                    std::cout << move.to_string();
                    if (b.read(move.end_position_) == std::nullopt) {
                        if (!move.is_en_passant_) {
                            return false;
                        }
                        return move.end_position_ == b.en_passant_target_square_;
                    }
                    else if (move.is_en_passant_) {
                        //if the move happens to be a regular capture (on a non-empty square) and was labelled en passant by the pgn parser or move generator,
                        //then we know it is not an actual en passant move. we set the flag to false, and relaunch it all the way up in the is_legal_procedure
                        //because being "potentially en passant" made us skip some verifications on this move, that we want to do now.
                        std::cout << move.to_string() << " is actually not en passant " << "\n";
                        move.is_en_passant_ = false;
                        return is_move_legal(b, move);
                    }
                    if (move.start_position_.file_get() - 1 == move.end_position_.file_get() && move.start_position_.rank_get() + rank_direction == move.end_position_.rank_get())
                        return true;

                    if (move.start_position_.file_get() + 1 == move.end_position_.file_get() && move.start_position_.rank_get() + rank_direction == move.end_position_.rank_get())
                        return true;
                }
                return false;
        }
        std::cout << "ok good: " + move.to_string();
        */return true;
    }

    bool MoveLegalityChecker::is_move_legal_ROOK(const Chessboard& b, Move& move) {
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
                    break;
                Fi += 1;
            }
            return move.start_position_.file_get() + Fi == move.end_position_.file_get();
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
                    break;
                Fi += 1;
            }
            return move.start_position_.file_get() - Fi == move.end_position_.file_get();
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
                    break;
                Ri += 1;
            }
            return move.start_position_.rank_get() + Ri == move.end_position_.rank_get();
        }

        else
        {
            while (move.start_position_.rank_get() - Ri != move.end_position_.rank_get())
            {
                if (move.start_position_.rank_get() - Ri == Rank::OUTOFBOUNDS)
                {
                    return false;
                }
                if (b.board_((int)move.start_position_.file_get(), (int)move.start_position_.rank_get() - Ri).has_value()) {
                    /*if (move.piece_ == PieceType::QUEEN) {
                        auto piece = b.board_((int)move.start_position_.file_get(), (int)move.start_position_.rank_get() - Ri);
                        auto p = move.start_position_.rank_get() - Ri == move.end_position_.rank_get() && piece.has_value();
                        std::cout << p << " " << Ri << " " << move.to_string();
                    }*/
                    break;
                }
                Ri += 1;
            }
            return move.start_position_.rank_get() - Ri == move.end_position_.rank_get();
        }
    }

    bool MoveLegalityChecker::is_move_legal(const Chessboard& b, Move &move) {
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
