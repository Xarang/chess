#pragma once

#include <boost/numeric/ublas/matrix.hpp>
#include <map>
#include <optional>
#include <vector>


#include "color.hh"

#include "move.hh"

#include "piece.hh"
#include "piece-type.hh"
#include "position.hh"

namespace board {


    /*
    ** Implementation of Chessboard using a matrix and a list of pieces
    */
    class Chessboard {


        private:

        //matrix representation of the board [FILE][RANK]
        //squares hold a nullptr if empty, a pointer to a piece in the piece list otherwise
        boost::numeric::ublas::matrix<std::optional<Piece>> board_ = boost::numeric::ublas::matrix<std::optional<Piece>>(8,8);
        //all our pieces
        std::map<std::pair<Color, PieceType>, std::vector<Piece>> pieces_;

        //whose turn is it ?
        bool is_white_turn_ = true;

        //keep track of some events to assess legality of some moves
        bool did_white_king_castling_ = false;
        bool did_white_queen_castling_ = false;
        bool did_black_king_castling_ = false;
        bool did_black_queen_castling_ = false;

        //turns elapsed since start
        unsigned int current_turn_ = 0;

        //turns elapsed since a piece was last taken or a pawn moved
        std::vector<int>turns_since_last_piece_taken_or_pawn_moved_ = {0};

        std::vector<std::optional<Position>> en_passant_target_square_ = {std::nullopt};
        //std::optional<Position> old_en_passant_target_square_ = std::nullopt;
        
        //map containing hashed representations of all boards since start (to_string'ed)
        //if a key has 3 values it means the '3 fold' rule applies and the game is a draw
        std::unordered_map<std::string, int> all_boards_since_start_ = std::unordered_map<std::string, int>();

        std::vector<Piece> last_piece_capture;

        //these 3 methods are used by do_move
        void remove_piece(const Piece& p);
        void move_piece(const Piece& p, Position new_position);
        void promote_piece(const Piece& p, PieceType type);

        //theses methods are used by undo_move
        void add_piece(std::optional<Piece> p);
        void undo_move_piece(const Piece& p, Position old_position);
        void undo_promote_piece(const Piece& p, PieceType type);

        public:

        bool operator==(Chessboard b) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    if (!(board_(i, j) == b.board_(i, j))) {
                        std::cout << "the elt at " << i << " " << j << " is different\n";
                        return false;
                    }
                }
            }
            for (auto elt : pieces_) {
                if (find(b.pieces_.begin(), b.pieces_.end(), elt) == b.pieces_.end()) {
                    return false;
                }
            }
            if (is_white_turn_ != b.is_white_turn_) {
                std::cout << "Is_whit_turn is different " << is_white_turn_ << " vs " << b.is_white_turn_;
                return false;
            }
            if (did_white_queen_castling_ != b.did_white_queen_castling_)
            {
                std::cout << "did_white_queen_castling is different " << did_white_queen_castling_ << " vs " << b.did_white_queen_castling_;
                return false;
            }
            if (did_white_king_castling_ != b.did_white_king_castling_)
            {
                std::cout << "did_white_king_castling is different " << did_white_king_castling_ << " vs " << b.did_white_king_castling_;
                return false;
            }
            if (did_black_queen_castling_ != b.did_black_queen_castling_)
            {
                std::cout << "did_black_queen_castling is different " << did_black_queen_castling_ << " vs " << b.did_black_queen_castling_;
                return false;
            }
            if (did_black_king_castling_ != b.did_black_king_castling_)
            {
                std::cout << "did_black_king_castling is different " << did_black_king_castling_ << " vs " << b.did_black_king_castling_;
                return false;
            }
            if (current_turn_ != b.current_turn_)
            {
                std::cout << "current is different "  << current_turn_ << " vs " << b.current_turn_;
                return false;
            }
            if (turns_since_last_piece_taken_or_pawn_moved_[turns_since_last_piece_taken_or_pawn_moved_.size() - 1]
            != b.turns_since_last_piece_taken_or_pawn_moved_[b.turns_since_last_piece_taken_or_pawn_moved_.size() - 1])
            {
                std::cout << "turns_since_last_piece_taken_or_pawn_moved_ is different "
                << turns_since_last_piece_taken_or_pawn_moved_[turns_since_last_piece_taken_or_pawn_moved_.size() - 1] << " vs "
                << b.turns_since_last_piece_taken_or_pawn_moved_[b.turns_since_last_piece_taken_or_pawn_moved_.size() - 1];
                return false;
            }
            for (unsigned long i = 0; i < en_passant_target_square_.size(); i++)
            {
                if (en_passant_target_square_[i].has_value())
                {
                    if (en_passant_target_square_[i].value() != b.en_passant_target_square_[i].value())
                    {
                        std::cout << "en passant target suqare are different\n";
                    }
                }
            }
            return true;
        }

        // constructors 

        //default constructor
        Chessboard() {
                //White Pawns
                std::vector<Piece> pieces;
                pieces.push_back(Piece(Position(File::A, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::B, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::C, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::D, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::E, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::F, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::G, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::H, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.insert({std::make_pair(Color::WHITE, PieceType::PAWN), pieces});
                pieces.clear();
                //White Specials
                pieces.push_back(Piece(Position(File::A, Rank::ONE), Color::WHITE, PieceType::ROOK));
                pieces.push_back(Piece(Position(File::H, Rank::ONE), Color::WHITE, PieceType::ROOK));
                pieces_.insert({std::make_pair(Color::WHITE, PieceType::ROOK), pieces});
                pieces.clear();
                pieces.push_back(Piece(Position(File::F, Rank::ONE), Color::WHITE, PieceType::BISHOP));
                pieces.push_back(Piece(Position(File::C, Rank::ONE), Color::WHITE, PieceType::BISHOP));
                pieces_.insert({std::make_pair(Color::WHITE, PieceType::BISHOP), pieces});
                pieces.clear();
                pieces.push_back(Piece(Position(File::B, Rank::ONE), Color::WHITE, PieceType::KNIGHT));
                pieces.push_back(Piece(Position(File::G, Rank::ONE), Color::WHITE, PieceType::KNIGHT));
                pieces_.insert({std::make_pair(Color::WHITE, PieceType::KNIGHT), pieces});
                pieces.clear();
                pieces.push_back(Piece(Position(File::D, Rank::ONE), Color::WHITE, PieceType::QUEEN));
                pieces_.insert({std::make_pair(Color::WHITE, PieceType::QUEEN), pieces});
                pieces.clear();
                pieces.push_back(Piece(Position(File::E, Rank::ONE), Color::WHITE, PieceType::KING));
                pieces_.insert({std::make_pair(Color::WHITE, PieceType::KING), pieces});
                pieces.clear();
                //Black Pawns
                pieces.push_back(Piece(Position(File::A, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::B, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::C, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::D, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::E, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::F, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::G, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces.push_back(Piece(Position(File::H, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.insert({std::make_pair(Color::BLACK, PieceType::PAWN), pieces});
                pieces.clear();
                //Black Specials
                pieces.push_back(Piece(Position(File::A, Rank::EIGHT), Color::BLACK, PieceType::ROOK));
                pieces.push_back(Piece(Position(File::H, Rank::EIGHT), Color::BLACK, PieceType::ROOK));
                pieces_.insert({std::make_pair(Color::BLACK, PieceType::ROOK), pieces});
                pieces.clear();
                pieces.push_back(Piece(Position(File::B, Rank::EIGHT), Color::BLACK, PieceType::KNIGHT));
                pieces.push_back(Piece(Position(File::G, Rank::EIGHT), Color::BLACK, PieceType::KNIGHT));
                pieces_.insert({std::make_pair(Color::BLACK, PieceType::KNIGHT), pieces});
                pieces.clear();
                pieces.push_back(Piece(Position(File::C, Rank::EIGHT), Color::BLACK, PieceType::BISHOP));
                pieces.push_back(Piece(Position(File::F, Rank::EIGHT), Color::BLACK, PieceType::BISHOP));
                pieces_.insert({std::make_pair(Color::BLACK, PieceType::BISHOP), pieces});
                pieces.clear();
                pieces.push_back(Piece(Position(File::D, Rank::EIGHT), Color::BLACK, PieceType::QUEEN));
                pieces_.insert({std::make_pair(Color::BLACK, PieceType::QUEEN), pieces});
                pieces.clear();
                pieces.push_back(Piece(Position(File::E, Rank::EIGHT), Color::BLACK, PieceType::KING));
                pieces_.insert({std::make_pair(Color::WHITE, PieceType::KING), pieces});

                //fill the initial matrix
                for (auto vector : pieces_) {
                    for (auto piece : vector.second) {
                        (*this)[Position(piece.position_.file_get(), piece.position_.rank_get())] = piece;
                    }
                }
                all_boards_since_start_.insert(std::pair<std::string, int>(to_string(), 1));
        }

        //copy constructor (overloaded for assertions)
        Chessboard(const Chessboard& other) = default;

        //FEN string based constructor
        Chessboard(std::string fen_string);

        static Chessboard parse_uci(std::string uci_position);

        //main methods
        void do_move(Move);
        bool is_move_legal(Move&, bool check_self_check = true);
        std::list<Move> generate_legal_moves(bool check_self_check = true);
        bool is_check();
        bool is_checkmate();
        bool is_draw();
        void undo_move(Move);
        std::optional<Piece>& operator[](Position p);


        //Getters
        std::map<std::pair<Color, PieceType>, std::vector<Piece>> get_pieces() {
            return pieces_;
        }

        //utils
        std::string to_string() const;
        Color whose_turn_is_it() const { return is_white_turn_ ? Color::WHITE : Color::BLACK; }
        const std::optional<Piece> read(Position p) const; //same as operator[], but read-only

        void change_turn(){ is_white_turn_ = !is_white_turn_; }
        friend class MoveLegalityChecker;

        //make a copy of the board with the move passed as argument executed
        Chessboard project(Move move) const;

    };

    class MoveLegalityChecker {
        public:
        static bool is_move_legal_QUEEN(Chessboard& b, Move& move);
        static bool is_move_legal_KING(Chessboard& b, Move& move);
        static bool is_move_legal_ROOK(Chessboard& b, Move& move);
        static bool is_move_legal_KNIGHT(Chessboard& b, Move& move);
        static bool is_move_legal_BISHOP(Chessboard& b, Move& move);
        static bool is_move_legal_PAWN(Chessboard& b, Move& move);
        static bool is_move_legal(Chessboard& b, Move& move);
    };

    //extern Chessboard myBoard;

}
