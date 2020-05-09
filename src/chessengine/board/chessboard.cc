#include <err.h>
#include <sstream>
#include "chessboard.hh"
#include "piece-type.hh"

namespace board {

    void Chessboard::do_move(Move move) {
        current_turn_++;
        turns_since_last_piece_taken_or_pawn_moved_++;
        int j = 0;

        //TODO: hash the current state of the board (string ?) and store it inside the all_boards_since_start_ multimap

        
        //if the board previously had a 'en passant target square', this do_move "consumes" it
        en_passant_target_square_ = std::nullopt;

        auto ite = pieces_.begin();
        ++ite;
        while (ite != pieces_.end())
        {
            if (ite->position_ != move.end_position_)
            {
                ++ite;
            } else {
                turns_since_last_piece_taken_or_pawn_moved_ = 0;
                pieces_.erase(ite);
                break;
            }
        }

        for (unsigned  long i = 0; i < pieces_.size(); i++)
        {
            if (pieces_[i].position_ == move.start_position_)
            {
                if (move.piece_ == PieceType::PAWN) {
                    turns_since_last_piece_taken_or_pawn_moved_ = 0;
                }
                pieces_[i].position_ = move.end_position_;
                pieces_[i].has_already_moved_ = true;
                j = i;
                break;
            }
        }

        board_((int)move.end_position_.file_get(), (int)move.end_position_.rank_get()) = std::make_optional(pieces_[j]);
        board_((int)move.start_position_.file_get(), (int)move.start_position_.rank_get()) = std::nullopt;

        if (move.promotion_.has_value())
        {
            (*this)[move.end_position_] = Piece(move.end_position_, whose_turn_is_it(), move.promotion_.value());
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
        }
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
        return false;
    }

    bool Chessboard::is_checkmate() {
        //is check + project all your legal moves as long as you don't find one in which you are not checked. if you can't find one, you are checkmated
        return false;
    }

    bool Chessboard::is_draw() {
        //player has 0 legal moves (but is not checked)
        //or
        //same board configuration happened 3 times
        //or
        //no pawn moved or piece captured in last 50 turns
        return false;
    }

    std::optional<Piece>& Chessboard::operator[](Position pos) {
        return board_((int)pos.file_get(), (int)pos.rank_get());
    }

    std::string Chessboard::to_string() {
        //TODO: output a FEN string representing the board (https://fr.wikipedia.org/wiki/Notation_Forsyth-Edwards)
        throw "not implemented";
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
