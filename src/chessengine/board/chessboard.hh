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

        //matrix representation of the board [FILE][RANK]
        boost::numeric::ublas::matrix<std::optional<struct Piece>> board_;

        //all our pieces
        std::vector<struct Piece> pieces_;

        //whose turn is it ?
        bool is_white_turn_;

        //keep track of some events to assess legality of some moves
        bool did_white_king_castling_;
        bool did_white_queen_castling_;
        bool did_black_king_castling_;
        bool did_black_queen_castling_;

        //turns elapsed since start
        unsigned int current_turn_;

        //turns elapsed since a piece was last taken or a pawn moved
        unsigned int turns_since_last_piece_taken_or_pawn_moved_;

        std::optional<Position> en_passant_target_square_;

        //map containing hashed representations of all boards since start (to_string'ed)
        //if a key has 3 values it means the '3 fold' rule applies and the game is a draw
        std::list<std::string> all_boards_since_start_;


        public:
        Chessboard() : board_(8,8), pieces_(),
            is_white_turn_(true), did_white_king_castling_(false), did_white_queen_castling_(false),
            did_black_king_castling_(false), did_black_queen_castling_(false), current_turn_(0),
            turns_since_last_piece_taken_or_pawn_moved_(0), en_passant_target_square_(), all_boards_since_start_() {
                //White Pawns
                pieces_.push_back(Piece(Position(File::A, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::B, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::C, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::D, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::E, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::F, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::G, Rank::TWO), Color::WHITE, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::H, Rank::TWO), Color::WHITE, PieceType::PAWN));
                //White Specials
                pieces_.push_back(Piece(Position(File::A, Rank::ONE), Color::WHITE, PieceType::ROOK));
                pieces_.push_back(Piece(Position(File::B, Rank::ONE), Color::WHITE, PieceType::KNIGHT));
                pieces_.push_back(Piece(Position(File::C, Rank::ONE), Color::WHITE, PieceType::BISHOP));
                pieces_.push_back(Piece(Position(File::D, Rank::ONE), Color::WHITE, PieceType::QUEEN));
                pieces_.push_back(Piece(Position(File::E, Rank::ONE), Color::WHITE, PieceType::KING));
                pieces_.push_back(Piece(Position(File::F, Rank::ONE), Color::WHITE, PieceType::BISHOP));
                pieces_.push_back(Piece(Position(File::G, Rank::ONE), Color::WHITE, PieceType::KNIGHT));
                pieces_.push_back(Piece(Position(File::H, Rank::ONE), Color::WHITE, PieceType::ROOK));
                //Black Pawns
                pieces_.push_back(Piece(Position(File::A, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::B, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::C, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::D, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::E, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::F, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::G, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                pieces_.push_back(Piece(Position(File::H, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
                //Black Specials
                pieces_.push_back(Piece(Position(File::A, Rank::EIGHT), Color::BLACK, PieceType::ROOK));
                pieces_.push_back(Piece(Position(File::B, Rank::EIGHT), Color::BLACK, PieceType::KNIGHT));
                pieces_.push_back(Piece(Position(File::C, Rank::EIGHT), Color::BLACK, PieceType::BISHOP));
                pieces_.push_back(Piece(Position(File::D, Rank::EIGHT), Color::BLACK, PieceType::QUEEN));
                pieces_.push_back(Piece(Position(File::E, Rank::EIGHT), Color::BLACK, PieceType::KING));
                pieces_.push_back(Piece(Position(File::F, Rank::EIGHT), Color::BLACK, PieceType::BISHOP));
                pieces_.push_back(Piece(Position(File::G, Rank::EIGHT), Color::BLACK, PieceType::KNIGHT));
                pieces_.push_back(Piece(Position(File::H, Rank::EIGHT), Color::BLACK, PieceType::ROOK));

                //fill the initial matrix
                for (Piece piece : pieces_) {
                    board_((int)piece.position_.file_get(), (int)piece.position_.rank_get()) = piece;
                    //todo: ^ make sure cast to int works, and file and rank are in good order
                }
                all_boards_since_start_.push_front(to_string());
        }

        //recreates a chessboard from an ongoing game using a fen string
        Chessboard(std::string fen_string);

        void do_move_en_passant(Move);
        void do_move(Move);
        bool is_move_legal(Move, bool check_self_check = true);
        std::list<Move> generateLegalMoves(bool check_self_check = true);
        bool is_check();
        bool is_checkmate();
        bool is_draw();
        std::optional<Piece>& operator[](Position p);

        //utils
        std::string to_string();
        Color whose_turn_is_it() { return is_white_turn_ ? Color::WHITE : Color::BLACK; }

        friend class MoveLegalityChecker;

        Chessboard(const Chessboard& other) = default;
        //make a copy of the board with the move passed as argument executed
        Chessboard project(Move move);

        void printMove(Move move);
    };

    class MoveLegalityChecker {
        public:
        static bool is_move_legal_QUEEN(Chessboard b, Move move);
        static bool is_move_legal_KING(Chessboard b, Move move);
        static bool is_move_legal_ROOK(Chessboard b, Move move);
        static bool is_move_legal_KNIGHT(Chessboard b, Move move);
        static bool is_move_legal_BISHOP(Chessboard b, Move move);
        static bool is_move_legal_PAWN(Chessboard b, Move move);
    };



}
