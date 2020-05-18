#include <sstream>
#include "chessboard.hh"
#include "piece-type.hh"
#include "piece.hh"

namespace board {

    bool Chessboard::is_move_legal(Move &move, bool check_self_check) {

        //do not consider moves that have OOB positions
        if (move.start_position_.file_get() == File::OUTOFBOUNDS ||
            move.start_position_.rank_get() == Rank::OUTOFBOUNDS
            || move.end_position_.file_get() == File::OUTOFBOUNDS ||
            move.end_position_.rank_get() == Rank::OUTOFBOUNDS) {
            return false;
        }
        if (past_moves_en_passant_target_squares_.back().has_value() &&
            move.end_position_ ==
            past_moves_en_passant_target_squares_.back().value()
            && move.is_capture_ && move.piece_ == PieceType::PAWN) {
            //this is the place where en passant moves are flagged are so. no en passant moves should be created before this.
            move.is_en_passant_ = true;
        }

        //do not consider moves that move a piece that does not exist
        if (!(*this)[move.start_position_].has_value())
            return false;

        auto piece = (*this)[move.end_position_];
        if (piece.has_value()) {
            if (!move.is_capture_) {
                return false;
            }
            //do not consider moves that end on a square occupied by an allied piece
            if (piece.value().color_ == whose_turn_is_it())
                return false;
        } else if (move.is_capture_ && !move.is_en_passant_) {
            //do not consider capture moves that end on an empty square...except for en-passant
            return false;
        }
        //check if the movement is 'correct' relative to the piece type, and potential piece blocking the movement
        if (!MoveLegalityChecker::is_move_legal(*this, move)) {
            return false;
        }
        //now that we know the move is 'valid', we check if it is illegal due to " can not check yourself " clause
        //check if this move would make/leave the king vulnerable
        //if you are already checked, this is not an option
        if (check_self_check) {

            if (move.is_king_castling_ || move.is_queen_castling_) {
                if (is_check()) {
                    return false;
                }
                //check that the castling is not passing through other pieces
                auto king = pieces_[{PieceType::KING, whose_turn_is_it()}].front();
                auto moves_to_castling = MoveBuilder::generate_castling_decomposition(king, move.is_king_castling_);
                bool was_checked_on_the_way = false;
                for (auto it = moves_to_castling.begin(); it != moves_to_castling.end(); it++) {
                    //we know that these tiles are free
                    do_move(*it, false);
                    if (is_check()) {
                        was_checked_on_the_way = true;
                    }
                }
                for (auto it = moves_to_castling.rbegin(); it != moves_to_castling.rend(); it++) {
                    //we know that these tiles are free
                    undo_move(*it, false);
                }
                return !was_checked_on_the_way;
            } //castlings handled

            do_move(move, false);
            bool checked = is_check();
            undo_move(move, false);
            return !checked;

        }

        return true;
        //auto projection = project(move, false && "do not change turn");
        //if (projection.is_check()) {
        //    return false;
        //}

    }

    std::list<Move> Chessboard::generate_legal_moves(bool check_self_check) {
        std::list<Move> allMoves;

        //build the list of all "potential" moves, not accounting for OOB and blocked paths
        for (auto piece_set : pieces_) {
            if (piece_set.first.second == whose_turn_is_it()){
                for (auto piece : piece_set.second) {
                    std::list<Move> pieceMoves = piece.getAllPotentialMoves();
                    for (auto move : pieceMoves) {
                        if (move.is_capture_)
                        {
                            allMoves.push_front(move);
                        }
                        else
                        {
                            allMoves.push_back(move);
                        }
                    }
                }
            }
        }

        allMoves.remove_if([this, check_self_check](Move &m) {
            return !this->is_move_legal(m, check_self_check);
        });
        return allMoves;
    }

//copies the board and perform a move on it
    Chessboard Chessboard::project(Move move, bool change_turn) const {
        auto copy = Chessboard(*this);
        copy.do_move(move, change_turn);
        return copy;
    }

    bool Chessboard::is_check() {

        //generate legal moves for opponent and check if one on them captures your king.
        change_turn();
        std::list<Move> opponent_moves = generate_legal_moves(
                false); //this false means that this call to generate_legal_moves will not check for check itself (since the other player has initiative anyway)
        change_turn();

        auto king = pieces_[{PieceType::KING, whose_turn_is_it()}].front();

        for (auto move : opponent_moves) {
            if (move.end_position_.file_get() == king.position_.file_get() &&
                move.end_position_.rank_get() == king.position_.rank_get()) {
                return true;
            }
        }
        return false;
    }

    bool Chessboard::is_checkmate() {
        //is check + project all your legal moves as long as you don't find one in which you are not checked. if you can't find one, you are checkmated
        if (!is_check()) {
            return false;
        }

        std::list<Move> moves = generate_legal_moves();
        for (auto move : moves) {
            if (is_move_legal(move)) {
                return false;
            }
        }
        return true;
    }

    bool Chessboard::is_draw() {
        //player has 0 legal moves (but is not checked)
        //or
        //same board configuration happened 3 times (handled elsewhere)
        //or
        //no pawn moved or piece captured in last 50 turns

        std::list<Move> legal_moves = generate_legal_moves();
        if (legal_moves.empty() && !is_check()) {
            return true;
        }
        if (past_moves_halfmove_clocks_.back() >= 50) {
            return true;
        }
        return false;
    }

    std::optional<Piece> &Chessboard::operator[](Position pos) {
        return board_((int) pos.file_get(), (int) pos.rank_get());
    }

//same as [], but read only
    const std::optional<Piece> Chessboard::read(Position pos) const {
        if (pos.file_get() == File::OUTOFBOUNDS ||
            pos.rank_get() == Rank::OUTOFBOUNDS)
            return std::nullopt;
        return board_((int) pos.file_get(), (int) pos.rank_get());
    }


    std::string Chessboard::to_string() const {
        //outputs a FEN string representing the board (https://fr.wikipedia.org/wiki/Notation_Forsyth-Edwards)
        std::string res = "";
        board::Rank currRank = Rank::EIGHT;
        for (size_t i = 0; i < 8; i++) {
            board::File currFile = File::A;
            int empty_counter = 0;
            for (size_t j = 0; j < 8; j++) {
                Position myPos(currFile, currRank);
                char symbol = ' ';

                if (read(myPos).has_value()) {
                    auto myPiece = read(myPos).value();
                    symbol = myPiece.to_char_fen();
                    if (empty_counter != 0)
                        res += std::to_string(empty_counter);
                    res += symbol;
                    empty_counter = 0;
                } else
                    empty_counter++;
                currFile = currFile + 1;

            }
            if (empty_counter != 0)
                res += std::to_string(empty_counter);
            if (i != 7) {
                res += "/";
            }
            currRank = currRank - 1;
        }
        //std::cout << res << "\n";
        return res;
    }


/*
** build a chessboard from a fen string representing a board state
*/
    Chessboard::Chessboard(std::string fen_string) : board_(8, 8) {

        //string is fen representation

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

        int rankIndex = 7;
        for (auto rank : ranks) {
            int fileIndex = 0;
            for (auto it = rank.begin(); it != rank.end(); it++) {
                if (isdigit(*it)) {
                    fileIndex += (int) *it - '0';
                } else {
                    auto piece = (Piece(
                            Position((File) fileIndex, (Rank) rankIndex),
                            islower(*it) ? Color::BLACK : Color::WHITE,
                            char_to_piece(toupper(*it))));
                    if (std::find(initial_positions[*it].begin(),
                                  initial_positions[*it].end(),
                                  piece.position_) ==
                        initial_positions[*it].end()) {
                        piece.has_already_moved_ = true;
                    }
                    pieces_[{piece.type_, piece.color_}].emplace_back(piece);
                    fileIndex++;
                }
            }
            rankIndex--;
        }
        for (auto piece_set : pieces_) {
            for (auto piece : piece_set.second) {
                (*this)[piece.position_] = piece;
            }
        }


        //parse informations regarding the current state of the game
        //whose turn is it
        is_white_turn_ = fields[1] == "w";

        //castling situation
        did_black_king_castling_ = fields[2].find("k") == fields[2].npos;
        did_white_king_castling_ = fields[2].find("K") == fields[2].npos;
        did_black_queen_castling_ = fields[2].find("q") == fields[2].npos;
        did_white_queen_castling_ = fields[2].find("Q") == fields[2].npos;

        if (fields[3] != "-") {
            File f = (File) (fields[3].at(0) - 'a');
            Rank r = (Rank) (fields[3].at(1) - '1');
            past_moves_en_passant_target_squares_.push_back(
                    std::make_optional<Position>(f, r));
        }
    }


//private methods used to factorise basic move operations
//heavily asserted for now, to make sure they do what we want them to do

    bool Chessboard::operator==(Chessboard b) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (!(board_(i, j) == b.board_(i, j))) {
                    std::cout << "the piece in MAT[" << i << "][" << j
                              << "]  is different\n";
                    std::cout << "MAT[ " << i << "][" << j << "] :"
                              << (board_(i, j).has_value() ? board_(i,
                                                                    j)->to_string()
                                                           : "nullopt") << "\n";
                    std::cout << "other.MAT[ " << i << "][" << j << "] :"
                              << (b.board_(i, j).has_value() ? b.board_(i,
                                                                        j)->to_string()
                                                             : "nullopt")
                              << "\n";
                    return false;
                }
            }
        }
        for (auto piece_set : pieces_) {
            auto equivalent_piece_set = b.pieces_[piece_set.first];
            for (auto piece : piece_set.second) {
                if (std::find(equivalent_piece_set.begin(),
                              equivalent_piece_set.end(), piece) ==
                    equivalent_piece_set.end()) {
                    std::cout << "could not find piece: " << piece.to_string()
                              << " in other piece map\n";
                    return false;
                }
            }
        }

        if (is_white_turn_ != b.is_white_turn_) {
            std::cout << "Is_whit_turn is different " << is_white_turn_
                      << " vs "
                      << b.is_white_turn_;
            return false;
        }
        if (did_white_queen_castling_ != b.did_white_queen_castling_) {
            std::cout << "did_white_queen_castling is different "
                      << did_white_queen_castling_ << " vs "
                      << b.did_white_queen_castling_;
            return false;
        }
        if (did_white_king_castling_ != b.did_white_king_castling_) {
            std::cout << "did_white_king_castling is different "
                      << did_white_king_castling_ << " vs "
                      << b.did_white_king_castling_;
            return false;
        }
        if (did_black_queen_castling_ != b.did_black_queen_castling_) {
            std::cout << "did_black_queen_castling is different "
                      << did_black_queen_castling_ << " vs "
                      << b.did_black_queen_castling_;
            return false;
        }
        if (did_black_king_castling_ != b.did_black_king_castling_) {
            std::cout << "did_black_king_castling is different "
                      << did_black_king_castling_ << " vs "
                      << b.did_black_king_castling_;
            return false;
        }
        if (current_turn_ != b.current_turn_) {
            std::cout << "current is different " << current_turn_ << " vs "
                      << b.current_turn_;
            return false;
        }
        if (past_moves_halfmove_clocks_ != b.past_moves_halfmove_clocks_) {
            std::cout << "past_moves_halfmove_clocks_ different\n" << "\n";
            std::cout << "size: " << past_moves_halfmove_clocks_.size() << "\n";
            std::cout << "other: " << b.past_moves_halfmove_clocks_.size()
                      << "\n";
            return false;
        }
        if (last_pieces_captured_ != b.last_pieces_captured_) {
            std::cout << "last pieces captured different\n";
            std::cout << "size: " << last_pieces_captured_.size() << "\n";
            std::cout << "other: " << b.last_pieces_captured_.size() << "\n";
        }
        if (past_moves_halfmove_clocks_ != b.past_moves_halfmove_clocks_) {
            std::cout << "past_moves_halfmove_clocks_ different\n" << "\n";
            std::cout << "size: " << past_moves_halfmove_clocks_.size() << "\n";
            std::cout << "other: " << b.past_moves_halfmove_clocks_.size()
                      << "\n";
            return false;
        }

        return true;
    }


}
