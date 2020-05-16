//
// Created by xarang on 15/05/2020.
//

#include "chessboard.hh"

namespace board {

    //we assume parsed move is legal
    Move Chessboard::parse_uci_move(std::string uci_move) {
        Move move;

        Position start_position((File) uci_move.at(0) - 'a', (Rank) uci_move.at(1) - '1');
        Position end_position((File) uci_move.at(2) - 'a', (Rank) uci_move.at(3) - '1');

        move.piece_ = (*this)[start_position]->type_;
        auto color = (*this)[start_position]->color_;
        move.start_position_ = start_position;
        move.end_position_ = end_position;

        if (move.piece_ == PieceType::PAWN) {

            int direction = color == Color::BLACK ? -1 : 1;
            if (start_position.rank_get() + direction * 2 == end_position.rank_get()) {
                move.is_double_pawn_push_ = true;
                return move;
            } else if (past_moves_en_passant_target_squares_.back().has_value() &&
                       past_moves_en_passant_target_squares_.back().value() == end_position &&
                       end_position.file_get() != start_position.file_get()) {
                move.is_capture_ = true;
                move.is_en_passant_ = true;
                return move;
            }
        } else if (move.piece_ == PieceType::KING) {
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


    Chessboard Chessboard::parse_uci(std::string uci_position) {
        auto uci_string_stream = std::istringstream(uci_position);
        std::string s;
        uci_string_stream >> s;

        if (s == "position") {
            uci_string_stream >> s;
        } else {
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
                    } else
                        break;
                }
                board = Chessboard(fen);
            }
            catch (std::exception &e) {
                std::cerr << "error while recreating board position from uci fen string";
            }
        } else if (s == "startpos") {
            std::string moves_keyword;
            uci_string_stream >> moves_keyword;
        } else {
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


}