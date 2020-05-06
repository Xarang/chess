#pragma once

#include <boost/numeric/ublas/matrix.hpp>
#include <vector>


#include "color.hh"

#include "move.hh"

#include "piece.hh"
#include "piece-type.hh"
#include "position.hh"

namespace board {

    struct Chessboard {
        protected:
            bool is_white_turn_;
            bool did_white_king_castling_;
            bool did_white_queen_castling_;
            bool did_black_king_castling_;
            bool did_black_queen_castling_;
            unsigned int current_turn_;
            unsigned int turns_since_last_piece_taken_or_pawn_moved_;
        public:
            Chessboard() : is_white_turn_(true), did_white_king_castling_(false),
                did_white_queen_castling_(false), did_black_king_castling_(false),
                did_black_queen_castling_(false), current_turn_(0), turns_since_last_piece_taken_or_pawn_moved_(0) {

                };
            
            virtual void do_move(struct Move) = 0;
            virtual bool is_move_legal(struct Move) = 0;
            virtual bool is_check() = 0;
            virtual bool is_checkmate() = 0;
            virtual bool is_draw() = 0;
            virtual std::optional<Piece> operator[](Position p) = 0;
            std::string to_string();
    };


    struct ListBasedChessboard : public Chessboard {
        std::vector<Piece> pieces_;

        public:
        ListBasedChessboard() {
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
        }

        void do_move(Move);
        bool is_move_legal(Move);
        bool is_check();
        bool is_checkmate();
        bool is_draw();
        std::optional<Piece> operator[](Position p);
        std::string to_string();
    };

    struct MatrixBasedChessboard : public Chessboard {
        boost::numeric::ublas::matrix<std::optional<Piece>> board_;

        public:
        MatrixBasedChessboard() : board_(8,8) {
            //fill the initial matrix
           throw "not implemented";
        }

        void do_move(struct Move);
        bool is_move_legal(struct Move);
        bool is_check();
        bool is_checkmate();
        bool is_draw();
        std::optional<Piece> operator[](Position p);
        std::string to_string();
    };



}