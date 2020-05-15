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
        std::vector<Piece> pieces_ = std::vector<Piece>();

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

        // constructors 

        //default constructor
        Chessboard() {
                //White Pawns
                pieces_.emplace_back(Piece(Position(File::A, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::B, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::C, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::D, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::E, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::F, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::G, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::H, Rank::TWO), Color::WHITE, PieceType::PAWN));
                //White Specials
                pieces_.emplace_back(Piece(Position(File::A, Rank::ONE), Color::WHITE, PieceType::ROOK));
                pieces_.emplace_back(Piece(Position(File::B, Rank::ONE), Color::WHITE, PieceType::KNIGHT));
                pieces_.emplace_back(Piece(Position(File::C, Rank::ONE), Color::WHITE, PieceType::BISHOP));
                pieces_.emplace_back(Piece(Position(File::D, Rank::ONE), Color::WHITE, PieceType::QUEEN));
                pieces_.emplace_back(Piece(Position(File::E, Rank::ONE), Color::WHITE, PieceType::KING));
                pieces_.emplace_back(Piece(Position(File::F, Rank::ONE), Color::WHITE, PieceType::BISHOP));
                pieces_.emplace_back(Piece(Position(File::G, Rank::ONE), Color::WHITE, PieceType::KNIGHT));
                pieces_.emplace_back(Piece(Position(File::H, Rank::ONE), Color::WHITE, PieceType::ROOK));
                //Black Pawns
                pieces_.emplace_back(Piece(Position(File::A, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::B, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::C, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::D, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::E, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::F, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::G, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.emplace_back(Piece(Position(File::H, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                //Black Specials
                pieces_.emplace_back(Piece(Position(File::A, Rank::EIGHT), Color::BLACK, PieceType::ROOK));
                pieces_.emplace_back(Piece(Position(File::B, Rank::EIGHT), Color::BLACK, PieceType::KNIGHT));
                pieces_.emplace_back(Piece(Position(File::C, Rank::EIGHT), Color::BLACK, PieceType::BISHOP));
                pieces_.emplace_back(Piece(Position(File::D, Rank::EIGHT), Color::BLACK, PieceType::QUEEN));
                pieces_.emplace_back(Piece(Position(File::E, Rank::EIGHT), Color::BLACK, PieceType::KING));
                pieces_.emplace_back(Piece(Position(File::F, Rank::EIGHT), Color::BLACK, PieceType::BISHOP));
                pieces_.emplace_back(Piece(Position(File::G, Rank::EIGHT), Color::BLACK, PieceType::KNIGHT));
                pieces_.emplace_back(Piece(Position(File::H, Rank::EIGHT), Color::BLACK, PieceType::ROOK));

                //fill the initial matrix
                for (Piece piece : pieces_) {
                    (*this)[Position(piece.position_.file_get(), piece.position_.rank_get())] = piece;
                }
        }

        //copy constructor (overloaded for assertions)
        Chessboard(const Chessboard& other) = default;

        //FEN string based constructor
        Chessboard(std::string fen_string);
        bool operator==(Chessboard b);

        Move parse_uci_move(std::string uci_move);
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
        std::vector<Piece> get_pieces() {
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
