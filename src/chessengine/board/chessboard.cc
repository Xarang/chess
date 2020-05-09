#include <err.h>
#include <sstream>
#include "chessboard.hh"
#include "piece-type.hh"
#include "piece.hh"

namespace board {

    void Chessboard::do_move_en_passant(Move move) {
        Position position;
        if (is_white_turn_)
        {
            position = Position(move.end_position_.file_get(), move.end_position_.rank_get() - 1);

        } else
        {
            position = Position(move.end_position_.file_get(), move.end_position_.rank_get() + 1);
        }
        auto ite = pieces_.begin();
        while (ite != pieces_.end()) {
            if (ite->position_ != position) {
                ite++;
            } else {
                turns_since_last_piece_taken_or_pawn_moved_ = 0;
                (*this)[position] = std::nullopt;
                pieces_.erase(ite);
                break;
            }
        }
    }

    void Chessboard::do_move(Move move) {
        current_turn_++;
        turns_since_last_piece_taken_or_pawn_moved_++;
        //TODO: hash the current state of the board (string ?) and store it inside the all_boards_since_start_ multimap

        //if the board previously had a 'en passant target square', this do_move "consumes" it
        en_passant_target_square_ = std::nullopt;

        if ((*this)[move.end_position_].has_value()) {
            auto ite = pieces_.begin();
            while (ite != pieces_.end()) {
                if (ite->position_ != move.end_position_) {
                    ite++;
                } else {
                    turns_since_last_piece_taken_or_pawn_moved_ = 0;
                    pieces_.erase(ite);
                    break;
                }
            }
        }
        if (move.piece_ == PieceType::PAWN && move.is_en_passant_)
        {
            do_move_en_passant(move);
        }

        for (unsigned  long i = 0; i < pieces_.size(); i++)
        {
            if (pieces_[i].position_ == move.start_position_)
            {
                if (move.piece_ == PieceType::PAWN) {
                    turns_since_last_piece_taken_or_pawn_moved_ = 0;
                }

                //this is how to properly move a piece on the board
                pieces_[i].position_ = move.end_position_;
                pieces_[i].has_already_moved_ = true;
                (*this)[move.end_position_] = pieces_[i];
                (*this)[move.start_position_] = std::nullopt;


                //board_((int)move.end_position_.file_get(), (int)move.end_position_.rank_get()) = std::make_optional(pieces_[j]);
                //board_((int)move.start_position_.file_get(), (int)move.start_position_.rank_get()) = std::nullopt;

                if (move.promotion_.has_value())
                {
                    pieces_[i].type_ = move.promotion_.value();
                    (*this)[move.end_position_] = Piece(move.end_position_, whose_turn_is_it(), move.promotion_.value());
                }
                //j = i;
                break;
            }
        }


        if (move.is_double_pawn_push_) {
            //mark the square that can be "prise en passant"'ed next turn
            int direction = whose_turn_is_it() == Color::WHITE ? +1 : -1;
            en_passant_target_square_ = std::make_optional<Position>(move.start_position_.file_get(), move.start_position_.rank_get() + direction);
            //TODO: unsure about this, make sure it marks the good square as eligible en passant target for next move
        }
        if (move.is_king_castling_ || move.is_queen_castling_)
        {
            File rookFile;
            Rank rookRank;
            File endRookFile;
            if (move.is_queen_castling_) {
                rookFile = File::A;
                endRookFile = File::D;
            }
            else
            {
                rookFile = File::H;
                endRookFile = File::F;
            }
            if (is_white_turn_) {
                rookRank = Rank::ONE;
            }
            else {
                rookRank = Rank::EIGHT;
            }
            board_((int)endRookFile, (int)rookRank) = board_((int)rookFile, (int)rookRank);
            board_((int)rookFile, (int)rookRank) = std::nullopt;

            for (unsigned  long i = 0; i < pieces_.size(); i++)
            {
                if (pieces_[i].position_.file_get() == rookFile && pieces_[i].position_.rank_get() == rookRank)
                {
                   pieces_[i].position_ = Position(endRookFile, rookRank);
                }
            }
        }
        all_boards_since_start_.push_front(to_string());
        is_white_turn_ = !is_white_turn_;
    }

    bool Chessboard::is_move_legal(Move move) {

        if (move.start_position_.file_get() == File::OUTOFBOUNDS || move.start_position_.rank_get() == Rank::OUTOFBOUNDS
            || move.end_position_.file_get() == File::OUTOFBOUNDS || move.end_position_.rank_get() == Rank::OUTOFBOUNDS) {
                return false;
            }

        if (!(*this)[move.start_position_].has_value())
            return false;

        //check if this move would make/leave the king vulnerable
        auto projection = project(move);
        projection.is_white_turn_ = !projection.is_white_turn_;
        if (projection.is_check()) {
            return false;
        }

        auto piece = (*this)[move.end_position_];
        if (piece.has_value())
        {
            if (piece.value().color_ == Color::WHITE && is_white_turn_)
                return false;
            if (piece.value().color_ == Color::BLACK && !is_white_turn_)
                return false;
        }
        else if (move.is_capture_) {
            if ((move.piece_ == PieceType::PAWN && !move.is_en_passant_) || move.piece_ != PieceType::PAWN)
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
        
        allMoves.remove_if([this](Move m){return !this->is_move_legal(m); });
    
        return allMoves;
    }

    //copies the board and perform a move on it
    Chessboard Chessboard::project(Move move) {
        auto copy = Chessboard(*this);
        copy.do_move(move);
        return copy;
    }

    bool Chessboard::is_check() {
        //generate legal moves for opponent and check if one on them captures your king.
        is_white_turn_ = !is_white_turn_;
        std::list<Move> opponent_moves = generateLegalMoves();
        is_white_turn_ = !is_white_turn_;

        File king_file = File::OUTOFBOUNDS;
        Rank king_rank = Rank::OUTOFBOUNDS;
        for (auto piece : pieces_)
        {
            if (piece.type_ == PieceType::KING && (bool)piece.color_ != is_white_turn_)
            {
                king_file = piece.position_.file_get();
                king_rank = piece.position_.rank_get();
                break;
            }
        }
        
        for (auto move : opponent_moves)
        {
            if (move.end_position_.file_get() == king_file &&
                move.end_position_.rank_get() == king_rank)
            {
                return true;
            }
        }
        return false;
    }

    bool Chessboard::is_checkmate() {
        //is check + project all your legal moves as long as you don't find one in which you are not checked. if you can't find one, you are checkmated
        if (!is_check())
        {
            return false;
        }

        std::list<Move> moves = generateLegalMoves();
        for (auto move : moves) {
            auto projection = project(move);
            projection.is_white_turn_ = !projection.is_white_turn_;
            if (!projection.is_check()) {
                return false;
            }
        }
        return true;
    }

    bool Chessboard::is_draw() {
        //player has 0 legal moves (but is not checked)
        //or
        //same board configuration happened 3 times
        //or
        //no pawn moved or piece captured in last 50 turns

        std::list<Move> legal_moves = generateLegalMoves();
        if (legal_moves.empty() && !is_check())
        {
            return true;
        }
        if (turns_since_last_piece_taken_or_pawn_moved_ >= 50)
        {
            return true;
        }

        std::string s = to_string();

        int count = 0;
        for (std::string board : all_boards_since_start_)
        {
            if (board == s)
            {
                count++;
                if (count == 3)
                {
                    return true;
                }
            }
        }
        return false;
    }

    std::optional<Piece>& Chessboard::operator[](Position pos) {
        return board_((int)pos.file_get(), (int)pos.rank_get());
    }

    std::string Chessboard::to_string() {
        //TODO: output a FEN string representing the board (https://fr.wikipedia.org/wiki/Notation_Forsyth-Edwards)
        std::string res = "";
        board::Rank currRank = Rank::EIGHT;
        for (size_t i = 0; i < 8; i++)
        {
            board::File currFile = File::A;
            int empty_counter = 0;
            for (size_t j = 0; j < 8; j++)
            {
                Position myPos(currFile, currRank);
                char symbol = ' ';

                if ((*this)[myPos].has_value()) {
                    auto myPiece = (*this)[myPos].value();

                    symbol = myPiece.piece_to_char_fen();
                    if (symbol == ' ')
                        empty_counter++;
                    else if (empty_counter != 0) {
                        res += std::to_string(empty_counter) + symbol;
                        empty_counter = 0;
                    }
                }
                res += "/";
            }
            
        }
        return res;
    }

    Chessboard::Chessboard(std::string fen_string) : board_(8,8) {
        auto fen_string_stream = std::istringstream(fen_string);
        std::vector<std::string> fields;
        std::string s;
        while (getline(fen_string_stream, s, ' ')) {
            fields.push_back(s);
        }

        //parse the board position
        auto piece_placement_stream = std::istringstream(fields[0]);
        std::vector<std::string> ranks;
        while (getline(piece_placement_stream, s, '/')) {
            ranks.push_back(s);
        }

        int rankIndex = 0;
        for (auto rank : ranks) {
            int fileIndex = 0;
            for (auto it = rank.begin(); it != rank.end(); it++) {
                if (isdigit(*it)) {
                    fileIndex += '0' - (int)*it;
                }
                else {
                    pieces_.push_back(Piece(Position((File)fileIndex, (Rank)rankIndex), islower(*it) ? Color::BLACK : Color::WHITE, char_to_piece(toupper(*it))));
                    fileIndex++;
                }
            }
            rankIndex++;
        }
        for (auto piece : pieces_) {
            (*this)[piece.position_] = piece;
        }

        //parse informations regarding the current state of the game
        is_white_turn_ = fields[1] == "w";
        did_black_king_castling_ = fields[2].find("k") != fields[2].npos;
        did_white_king_castling_ = fields[2].find("K") != fields[2].npos;
        did_black_queen_castling_ = fields[2].find("q") != fields[2].npos;
        did_white_queen_castling_ = fields[2].find("Q") != fields[2].npos;

        //TODO: add potential en-passant spot

    }
}
