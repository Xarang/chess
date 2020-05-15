//
// Created by xarang on 15/05/2020.
//

#include "chessboard.hh"

namespace board {

    void Chessboard::undo_move(Move move) {
        current_turn_ -= 1;
        /*if(turns_since_last_piece_taken_or_pawn_moved_[turns_since_last_piece_taken_or_pawn_moved_.size() - 1] > 0)
            turns_since_last_piece_taken_or_pawn_moved_[turns_since_last_piece_taken_or_pawn_moved_.size() - 1] -= 1;*/
        turns_since_last_piece_taken_or_pawn_moved_.pop_back();

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

        //Undo en passant (not sure about this)
        en_passant_target_square_.pop_back();

        /*int direction = whose_turn_is_it() == Color::WHITE ? +1 : -1;
        en_passant_target_square_ = std::make_optional<Position>(move.start_position_.file_get(), move.start_position_.rank_get() + direction);

        if (move.is_double_pawn_push_) {
            en_passant_target_square_ = std::nullopt;
        }*/

        if (move.promotion_.has_value()) {
            //Check undo_promotion use start_position because this move is undo
            undo_promote_piece((*this)[move.start_position_].value(), move.piece_);
        }

        if (move.is_capture_) {
            //if (!move.is_en_passant_) {
            auto piece = last_piece_capture.back();
            last_piece_capture.pop_back();
            add_piece(piece);
            (*this)[move.end_position_] = piece;
            /*} else {
                direction = whose_turn_is_it() == Color::WHITE ? 1 : -1;
                remove_piece((*this)[Position(en_passant_target_square_->file_get(), en_passant_target_square_->rank_get() - direction)].value());
            }*/
            //}
        }

        /*auto fen = to_string();
        if (all_boards_since_start_.find(fen) != all_boards_since_start_.end()) {
            all_boards_since_start_.insert_or_assign(fen, all_boards_since_start_.find(fen)->second + 1);
            //std::cout << fen << " ---> " << all_boards_since_start_.find(fen)->second << "\n";
        } else {
            all_boards_since_start_.insert(std::pair<std::string, int>(fen, 1));
        }*/
        is_white_turn_ = !is_white_turn_;
    }

    void Chessboard::undo_move_piece(const Piece &p, Position old_position) {
        assert(std::find(pieces_.begin(), pieces_.end(), p) != pieces_.end() && "queried piece not in piece list");
        //std::cout << "move piece from position " << p.position_.to_string() << " to position " << new_position.to_string() << "\n";
        auto piece_it = std::find(pieces_.begin(), pieces_.end(), p);
        assert(*piece_it == p && "fetched piece is not equal to the queried piece");
        piece_it->has_already_moved_ = false;
        piece_it->position_ = old_position;

        (*this)[p.position_] = std::nullopt;
        (*this)[piece_it->position_] = std::make_optional<Piece>(*piece_it);
        assert((*this)[piece_it->position_].value() == *piece_it && "copy constructor of piece did not copy properly");

        if (p.type_ == PieceType::PAWN) {
            turns_since_last_piece_taken_or_pawn_moved_.pop_back();
        }

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