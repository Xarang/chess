#include <sstream>
#include "chessboard.hh"
#include "piece-type.hh"
#include "piece.hh"

namespace board {

    void Chessboard::do_move(Move move) {
        current_turn_++;
        turns_since_last_piece_taken_or_pawn_moved_++;

        //handle capture (+ special behaviour for en-passant)
        if (move.is_capture_) {
            if (!move.is_en_passant_) {
                remove_piece((*this)[move.end_position_].value());
            } else {
                int direction = whose_turn_is_it() == Color::WHITE ? 1 : -1;
                remove_piece((*this)[Position(en_passant_target_square_->file_get(), en_passant_target_square_->rank_get() - direction)].value());
            }
        }   

        //handle movement (+ special behaviour for en-passant)
        if (!move.is_en_passant_)
        {
            move_piece((*this)[move.start_position_].value(), move.end_position_);
        } else {
            move_piece((*this)[move.start_position_].value(), en_passant_target_square_.value());
        }

        if (move.is_king_castling_ || move.is_queen_castling_)
        {
            File rookFile;
            Rank rookRank;
            File endRookFile;
            rookRank = whose_turn_is_it() == Color::WHITE ? Rank::ONE : Rank::EIGHT;
            if (move.is_queen_castling_) {
                rookFile = File::A;
                endRookFile = File::D;
            } else {
                rookFile = File::H;
                endRookFile = File::F;
            }
            Position rook_position(rookFile, rookRank);
            Position rook_destination(endRookFile, rookRank);

            //mark used castling
            std::pair<Position, bool&> l[] = {
                { Position(File::A, Rank::ONE), did_white_queen_castling_ },
                { Position(File::H, Rank::ONE), did_white_king_castling_ },
                { Position(File::A, Rank::EIGHT), did_black_queen_castling_ },
                { Position(File::H, Rank::EIGHT), did_black_king_castling_ },
            };
            for (auto association : l) {
                if (association.first == rook_position) {
                    association.second = true;
                }
            }
            move_piece((*this)[rook_position].value(), rook_destination);
        }

        //if the board previously had a 'en passant target square', it is now consumed
        en_passant_target_square_ = std::nullopt;

        if (move.is_double_pawn_push_) {
            //mark the square that can be "prise en passant"'ed next turn
            int direction = whose_turn_is_it() == Color::WHITE ? +1 : -1;
            en_passant_target_square_ = std::make_optional<Position>(move.start_position_.file_get(), move.start_position_.rank_get() + direction);
        }

        if (move.promotion_.has_value())
        {
            //move already happened so we use the endposition
            promote_piece((*this)[move.end_position_].value(), move.promotion_.value());
        }


        is_white_turn_ = !is_white_turn_;
    }

    bool Chessboard::is_move_legal(Move& move, bool check_self_check) {

        //do not consider moves that have OOB positions
        if (move.start_position_.file_get() == File::OUTOFBOUNDS || move.start_position_.rank_get() == Rank::OUTOFBOUNDS
            || move.end_position_.file_get() == File::OUTOFBOUNDS || move.end_position_.rank_get() == Rank::OUTOFBOUNDS) {
                return false;
            }
        if (en_passant_target_square_.has_value() && move.end_position_ == en_passant_target_square_.value() && move.is_capture_ && move.piece_ == PieceType::PAWN) {
            //this is the place where en passant moves are flagged are so. no en passant moves should be created before this.
            move.is_en_passant_ = true;
        }

        //do not consider moves that move a piece that does not exist
        if (!(*this)[move.start_position_].has_value())
            return false;

        auto piece = (*this)[move.end_position_];
        if (piece.has_value())
        {
            if (!move.is_capture_) {
                return false;
            }
            //do not consider moves that end on a square occupied by an allied piece
            if (piece.value().color_ == whose_turn_is_it())
                return false;
        }
        else if (move.is_capture_ && !move.is_en_passant_) {
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
            if ((move.is_king_castling_ || move.is_queen_castling_) && is_check()) {
                return false;
            }
            auto projection = project(move);
            projection.is_white_turn_ = !projection.is_white_turn_;
            if (projection.is_check()) {
                return false;
            }
        }

        return true;
      }

    std::list<Move> Chessboard::generate_legal_moves(bool check_self_check) {
        std::list<Move> allMoves;
 
        //build the list of all "potential" moves, not accounting for OOB and blocked paths
        for (auto piece : pieces_) {
            if (piece.color_ == whose_turn_is_it()) {
                std::list<Move> pieceMoves = piece.getAllPotentialMoves();
                for (auto move : pieceMoves) {
                    allMoves.push_front(move);
                }
            }
        }
        allMoves.remove_if([this, check_self_check](Move& m){return !this->is_move_legal(m, check_self_check); });
        return allMoves;
    }

    //copies the board and perform a move on it
    Chessboard Chessboard::project(Move move) const {
        auto copy = Chessboard(*this);
        copy.do_move(move);
        return copy;
    }

    bool Chessboard::is_check() {
        //std::cout << "is check ?" << "\n";
        //generate legal moves for opponent and check if one on them captures your king.
        change_turn();
        std::list<Move> opponent_moves = generate_legal_moves(false); //this false means that this call to generate_legal_moves will not check for check itself (since the other player has initiative anyway)
        change_turn();

        File king_file = File::OUTOFBOUNDS;
        Rank king_rank = Rank::OUTOFBOUNDS;
        for (auto piece : pieces_)
        {
            if (piece.type_ == PieceType::KING && piece.color_ == whose_turn_is_it())
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
        //same board configuration happened 3 times
        //or
        //no pawn moved or piece captured in last 50 turns

        std::list<Move> legal_moves = generate_legal_moves();
        if (legal_moves.empty() && !is_check())
        {
            return true;
        }
        if (turns_since_last_piece_taken_or_pawn_moved_ >= 50)
        {
            return true;
        }
        return false;
    }

    std::optional<Piece>& Chessboard::operator[](Position pos) {
        return board_((int)pos.file_get(), (int)pos.rank_get());
    }

    const std::optional<Piece> Chessboard::read(Position pos) const {
        return board_((int)pos.file_get(), (int)pos.rank_get());
    }


    std::string Chessboard::to_string() const {
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

                if (read(myPos).has_value()) {
                    auto myPiece = read(myPos).value();
                    symbol = myPiece.piece_to_char_fen();
                    if (empty_counter != 0)
                        res += std::to_string(empty_counter);
                    res += symbol;
                    empty_counter = 0;
                }
                else
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

        int rankIndex = 7;
        for (auto rank : ranks) {
            int fileIndex = 0;
            for (auto it = rank.begin(); it != rank.end(); it++) {
                if (isdigit(*it)) {
                    fileIndex += (int)*it - '0';
                }
                else {
                    pieces_.push_back(Piece(Position((File)fileIndex, (Rank)rankIndex), islower(*it) ? Color::BLACK : Color::WHITE, char_to_piece(toupper(*it))));
                    fileIndex++;
                }
            }
            rankIndex--;
        }
        for (auto piece : pieces_) {
            (*this)[piece.position_] = piece;
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
            File f = (File)(fields[3].at(0) - 'a');
            Rank r = (Rank)(fields[3].at(1) - '1');
            en_passant_target_square_ = std::make_optional<Position>(f, r);
        }
    }

    Chessboard Chessboard::parse_uci(std::string uci_position) {
        auto uci_string_stream = std::istringstream(uci_position);
        std::string s;
        uci_string_stream >> s;

        if (s == "position")
        {
            uci_string_stream >> s;
        }
        else
        {
            throw std::runtime_error("position string does not start with position");
        }
        auto board = Chessboard();

        if (s == "fen") {
            try {
                std::string fen;
                std::string word;
                while (uci_string_stream >> word) {
                    if (word != "moves") {
                        fen += word + " "; 
                    }
                    else
                        break;
                }
                board = Chessboard(fen);
            }
            catch (std::exception &e) {
                std::cerr << "error while recreating board position from uci fen string";
            }
        }
        else if (s == "startpos") {
            std::string moves_keyword;
            uci_string_stream >> moves_keyword;
        }
        else {
            throw std::runtime_error("unrecognised UCI position");
        }
    
        try {
            std::string move;
            while (uci_string_stream >> move) {
                Move parsed_uci_move = board.parse_uci_move(move);
                board.do_move(parsed_uci_move);
            }
            return board;
        }
        catch (std::exception &e) {
            std::cerr << "error while recreating board position from uci move list :" << e.what() << "\n";
        }

        return Chessboard();
    }

    //private methods used to factorise basic move operations

    //heavily asserted for now, to make sure they do what we want them to do

    //we assume parsed move is legal
    Move Chessboard::parse_uci_move(std::string uci_move) {
        Move move;

        Position start_position((File)uci_move.at(0) - 'a', (Rank)uci_move.at(1) - '1');
        Position end_position((File)uci_move.at(2) - 'a', (Rank)uci_move.at(3) - '1');

        move.piece_ = (*this)[start_position]->type_;
        auto color = (*this)[start_position]->color_;
        move.start_position_ = start_position;
        move.end_position_ = end_position;

        if (move.piece_ == PieceType::PAWN) {

            int direction = color == Color::BLACK ? -1 : 1;
            if (start_position.rank_get() + direction * 2 == end_position.rank_get()) {
                move.is_double_pawn_push_ = true;
                return move;
            }
            else if (en_passant_target_square_.has_value() && en_passant_target_square_.value() == end_position && end_position.file_get() != start_position.file_get()) {
                move.is_capture_ = true;
                move.is_en_passant_ = true;
                return move;
            }
        }
        else if (move.piece_ == PieceType::KING) {
            if (end_position.file_get() + 2 == start_position.file_get()) {
                move.is_king_castling_ = true;
                return move;
            }
            if (end_position.file_get() - 2 == start_position.file_get()) {
                move.is_queen_castling_ = true;
                return move;
            }
        }

        if ((*this)[end_position].has_value()) {
            move.is_capture_ = true;
        }

        return move;
    }

    void Chessboard::remove_piece(const Piece& p) {
        assert(std::find(pieces_.begin(), pieces_.end(), p) != pieces_.end() && "queried piece not in piece list");
        auto piece_it = std::find(pieces_.begin(), pieces_.end(), p);
        pieces_.erase(piece_it);
        (*this)[p.position_] = std::nullopt;
        turns_since_last_piece_taken_or_pawn_moved_ = 0;

        //make sure this did what we want
        assert(std::find(pieces_.begin(), pieces_.end(), p) == pieces_.end());
    }

    void Chessboard::move_piece(const Piece& p, Position new_position) {
        assert(std::find(pieces_.begin(), pieces_.end(), p) != pieces_.end() && "queried piece not in piece list");
        //std::cout << "move piece from position " << p.position_.to_string() << " to position " << new_position.to_string() << "\n";
        auto piece_it = std::find(pieces_.begin(), pieces_.end(), p);
        assert(*piece_it == p && "fetched piece is not equal to the queried piece");
        piece_it->has_already_moved_ = true;
        piece_it->position_ = new_position;

        (*this)[p.position_] = std::nullopt;
        (*this)[piece_it->position_] = std::make_optional<Piece>(*piece_it);
        assert((*this)[piece_it->position_].value() == *piece_it && "copy constructor of piece did not copy properly");

        if (p.type_ == PieceType::PAWN) {
            turns_since_last_piece_taken_or_pawn_moved_ = 0;
        }
        
        //make sure this did what we want
        assert((*this)[new_position] == *piece_it);
        assert((*this)[new_position]->type_ == p.type_);
        assert((*this)[new_position]->position_ != p.position_);
        assert((*this)[new_position]->color_ == p.color_);
    }

    void Chessboard::promote_piece(const Piece& p, PieceType new_type) {
        assert(std::find(pieces_.begin(), pieces_.end(), p) != pieces_.end() && "queried piece not in piece list");
        auto piece_it = std::find(pieces_.begin(), pieces_.end(), p);
        piece_it->type_ = new_type;
        (*this)[piece_it->position_] = std::make_optional<Piece>(*piece_it);
        
        assert(std::find(pieces_.begin(), pieces_.end(), p)->type_ == new_type && "promotion did not take effect in piece list");
        assert((*this)[p.position_]->type_ == new_type && "promotion did not take effect in piece list");
        assert(*std::find(pieces_.begin(), pieces_.end(), p) == (*this)[p.position_].value());
    }
}
