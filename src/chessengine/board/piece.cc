#include <iostream>

#include "piece.hh"
#include "piece-move-builder.hh"

namespace board {

    std::list<Move> Piece::getAllPotentialMoves() {
        switch (type_) {
            case PieceType::PAWN:
                return PieceMoveGenerator::Pawn(*this);
            case PieceType::BISHOP:
                return PieceMoveGenerator::Bishop(*this);
            case PieceType::KNIGHT:
                return PieceMoveGenerator::Knight(*this);
            case PieceType::ROOK:
                return PieceMoveGenerator::Rook(*this);
            case PieceType::QUEEN:
                return PieceMoveGenerator::Queen(*this);
            case PieceType::KING:
                return PieceMoveGenerator::King(*this);
            default:
                return std::list<Move>();
        }
    }

    char Piece::to_char_fen() const {
        if (this->color_ == Color::WHITE) {
            switch (this->type_) {
                case PieceType::PAWN:
                    return 'P';
                case PieceType::KNIGHT:
                    return 'N';
                case PieceType::BISHOP:
                    return 'B';
                case PieceType::ROOK:
                    return 'R';
                case PieceType::QUEEN:
                    return 'Q';
                case PieceType::KING:
                    return 'K';
                default:
                    return ' ';
            }
        }
        else {
            switch (this->type_) {
                case PieceType::PAWN:
                    return 'p';
                case PieceType::KNIGHT:
                    return 'n';
                case PieceType::BISHOP:
                    return 'b';
                case PieceType::ROOK:
                    return 'r';
                case PieceType::QUEEN:
                    return 'q';
                case PieceType::KING:
                    return 'k';
                default:
                    return ' ';
            }
        }
    }

    std::pair<PieceType, Color> Piece::piecetype_and_color_from_fen(char fen) {
        switch (fen) {
            case 'P':
                return {PieceType::PAWN, Color::WHITE };
            case 'N':
                return {PieceType::KNIGHT, Color::WHITE };
            case 'B':
                return {PieceType::BISHOP, Color::WHITE };
            case 'R':
                return {PieceType::ROOK, Color::WHITE };
            case 'K':
                return {PieceType::KING, Color::WHITE };
            case 'Q':
                return {PieceType::QUEEN, Color::WHITE };
            case 'p':
                return {PieceType::PAWN, Color::BLACK };
            case 'n':
                return {PieceType::KNIGHT, Color::BLACK };
            case 'b':
                return {PieceType::BISHOP, Color::BLACK };
            case 'r':
                return {PieceType::ROOK, Color::BLACK };
            case 'k':
                return {PieceType::KING, Color::BLACK };
            case 'q':
                return {PieceType::QUEEN, Color::BLACK };
            default:
                throw std::runtime_error("unrecognised fen char");
        }

    }

    std::string Piece::to_string() {
       return "[PIECE " + std::string(color_ == Color::WHITE ? "white" : "black") + "  " + std::string(1, to_char_fen()) + " at " + position_.to_string() + " (already moved ? " + (has_already_moved_ ? "yes" : "no") + ")]";
    }
}