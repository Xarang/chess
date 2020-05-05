#pragma once
#include <vector>
#include "piece.hh"
#include "position.hh"
#include "color.hh"
#include "piece-type.hh"

namespace board {

    struct Chessboard {
        std::vector<Piece> whites_;
        std::vector<Piece> blacks_;

        private:
        std::vector<Piece> generate_whites() {
            //Pawns
            whites_.push_back(Piece(Position(File::A, Rank::TWO), Color::WHITE, PieceType::PAWN));
            whites_.push_back(Piece(Position(File::B, Rank::TWO), Color::WHITE, PieceType::PAWN));
            whites_.push_back(Piece(Position(File::C, Rank::TWO), Color::WHITE, PieceType::PAWN));
            whites_.push_back(Piece(Position(File::D, Rank::TWO), Color::WHITE, PieceType::PAWN));
            whites_.push_back(Piece(Position(File::E, Rank::TWO), Color::WHITE, PieceType::PAWN));
            whites_.push_back(Piece(Position(File::F, Rank::TWO), Color::WHITE, PieceType::PAWN));
            whites_.push_back(Piece(Position(File::G, Rank::TWO), Color::WHITE, PieceType::PAWN));
            whites_.push_back(Piece(Position(File::H, Rank::TWO), Color::WHITE, PieceType::PAWN));
            
            //Specials
            whites_.push_back(Piece(Position(File::A, Rank::ONE), Color::WHITE, PieceType::ROOK));
            whites_.push_back(Piece(Position(File::B, Rank::ONE), Color::WHITE, PieceType::KNIGHT));
            whites_.push_back(Piece(Position(File::C, Rank::ONE), Color::WHITE, PieceType::BISHOP));
            whites_.push_back(Piece(Position(File::D, Rank::ONE), Color::WHITE, PieceType::QUEEN));
            whites_.push_back(Piece(Position(File::E, Rank::ONE), Color::WHITE, PieceType::KING));
            whites_.push_back(Piece(Position(File::F, Rank::ONE), Color::WHITE, PieceType::BISHOP));
            whites_.push_back(Piece(Position(File::G, Rank::ONE), Color::WHITE, PieceType::KNIGHT));
            whites_.push_back(Piece(Position(File::H, Rank::ONE), Color::WHITE, PieceType::ROOK));
        }

        std::vector<Piece> generate_blacks() {
            //Pawns
            blacks_.push_back(Piece(Position(File::A, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
            blacks_.push_back(Piece(Position(File::B, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
            blacks_.push_back(Piece(Position(File::C, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
            blacks_.push_back(Piece(Position(File::D, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
            blacks_.push_back(Piece(Position(File::E, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
            blacks_.push_back(Piece(Position(File::F, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
            blacks_.push_back(Piece(Position(File::G, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
            blacks_.push_back(Piece(Position(File::H, Rank::SEVEN), Color::BLACK, PieceType::PAWN));
            
            //Specials
            blacks_.push_back(Piece(Position(File::A, Rank::EIGHT), Color::BLACK, PieceType::ROOK));
            blacks_.push_back(Piece(Position(File::B, Rank::EIGHT), Color::BLACK, PieceType::KNIGHT));
            blacks_.push_back(Piece(Position(File::C, Rank::EIGHT), Color::BLACK, PieceType::BISHOP));
            blacks_.push_back(Piece(Position(File::D, Rank::EIGHT), Color::BLACK, PieceType::QUEEN));
            blacks_.push_back(Piece(Position(File::E, Rank::EIGHT), Color::BLACK, PieceType::KING));
            blacks_.push_back(Piece(Position(File::F, Rank::EIGHT), Color::BLACK, PieceType::BISHOP));
            blacks_.push_back(Piece(Position(File::G, Rank::EIGHT), Color::BLACK, PieceType::KNIGHT));
            blacks_.push_back(Piece(Position(File::H, Rank::EIGHT), Color::BLACK, PieceType::ROOK));
        }

        Chessboard() {
            generate_whites();
            generate_blacks();
        }
    };

}