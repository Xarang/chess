//
// Created by xarang on 15/05/2020.
//

#include "chessboard.hh"

namespace board {

    void Chessboard::undo_move(Move move) {
        assert(past_moves_halfmove_clocks_.size() >= 1);
        assert(past_moves_en_passant_target_squares_.size() >= 1);

        current_turn_ -= 1;

        //pop back last entry in all turn-by-turn registers
        past_moves_halfmove_clocks_.pop_back();
        past_moves_en_passant_target_squares_.pop_back();

        //undo the move
        undo_move_piece((*this)[move.end_position_].value(), move.start_position_);

        //undo the castling
        if (move.is_king_castling_ || move.is_queen_castling_) {
            File rookFile;
            Rank rookRank;
            File endRookFile;
            rookRank = whose_turn_is_it() == Color::WHITE ? Rank::ONE : Rank::EIGHT;
            if (move.is_queen_castling_) {
                rookFile = File::D;
                endRookFile = File::A;
            } else {
                rookFile = File::F;
                endRookFile = File::H;
            }
            Position rook_position(rookFile, rookRank);
            Position rook_destination(endRookFile, rookRank);

            //unmark used castling
            std::pair<Position, bool &> l[] = {
                    {Position(File::D, Rank::ONE),   did_white_queen_castling_},
                    {Position(File::F, Rank::ONE),   did_white_king_castling_},
                    {Position(File::D, Rank::EIGHT), did_black_queen_castling_},
                    {Position(File::F, Rank::EIGHT), did_black_king_castling_},
            };
            for (auto association : l) {
                if (association.first == rook_position) {
                    association.second = false;
                }
            }
            undo_move_piece((*this)[rook_position].value(), rook_destination);
        }


        if (move.promotion_.has_value()) {
            //Check undo_promotion use start_position because this move is undo
            undo_promote_piece((*this)[move.start_position_].value(), move.piece_);
        }

        if (move.is_capture_) {
            //if (!move.is_en_passant_) {
            auto piece = last_pieces_captured_.back();
            last_pieces_captured_.pop_back();
            add_piece(piece);
            (*this)[move.end_position_] = piece;
        }

        is_white_turn_ = !is_white_turn_;
    }

    void Chessboard::undo_move_piece(const Piece &p, Position old_position) {
        assert(std::find(pieces_.begin(), pieces_.end(), p) != pieces_.end() && "queried piece not in piece list");
        //std::cout << "move piece from position " << p.position_.to_string() << " to position " << new_position.to_string() << "\n";
        auto piece_it = std::find(pieces_.begin(), pieces_.end(), p);
        assert(*piece_it == p && "fetched piece is not equal to the queried piece");
        auto fen = p.to_char_fen();
        piece_it->has_already_moved_ = std::find(initial_positions[fen].begin(), initial_positions[fen].end(), old_position) == initial_positions[fen].end();
        piece_it->position_ = old_position;

        (*this)[p.position_] = std::nullopt;
        (*this)[piece_it->position_] = std::make_optional<Piece>(*piece_it);
        assert((*this)[piece_it->position_].value() == *piece_it && "copy constructor of piece did not copy properly");

        //make sure this did what we want
        assert((*this)[old_position] == *piece_it);
        assert((*this)[old_position]->type_ == p.type_);
        assert((*this)[old_position]->position_ != p.position_);
        assert((*this)[old_position]->color_ == p.color_);
    }

    void Chessboard::undo_promote_piece(const Piece &p, PieceType new_type) {
        assert(std::find(pieces_.begin(), pieces_.end(), p) != pieces_.end() && "queried piece not in piece list");
        auto piece_it = std::find(pieces_.begin(), pieces_.end(), p);
        piece_it->type_ = new_type;
        (*this)[piece_it->position_] = std::make_optional<Piece>(*piece_it);

        assert(std::find(pieces_.begin(), pieces_.end(), p)->type_ == new_type &&
               "promotion did not take effect in piece list");
        assert((*this)[p.position_]->type_ == new_type && "promotion did not take effect in piece list");
        assert(*std::find(pieces_.begin(), pieces_.end(), p) == (*this)[p.position_].value());
    }

}