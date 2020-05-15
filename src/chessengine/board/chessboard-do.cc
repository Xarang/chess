//
// Created by xarang on 15/05/2020.
//

#include "chessboard.hh"

namespace board {


    void Chessboard::do_move(Move move) {

        current_turn_ += 1;
        //turns_since_last_piece_taken_or_pawn_moved_[turns_since_last_piece_taken_or_pawn_moved_.size() - 1]+=1;
        auto temp = turns_since_last_piece_taken_or_pawn_moved_.back() + 1;
        turns_since_last_piece_taken_or_pawn_moved_.push_back(temp);

        //handle capture (+ special behaviour for en-passant)
        if (move.is_capture_) {
            if (!move.is_en_passant_) {
                remove_piece((*this)[move.end_position_].value());
            } else {
                int direction = whose_turn_is_it() == Color::WHITE ? 1 : -1;
                auto target = en_passant_target_square_.back().value();
                remove_piece((*this)[Position(target.file_get(), target.rank_get() - direction)].value());
            }
        }

        //handle movement (+ special behaviour for en-passant)
        if (!move.is_en_passant_) {
            move_piece((*this)[move.start_position_].value(), move.end_position_);
        } else {
            move_piece((*this)[move.start_position_].value(), en_passant_target_square_.back().value());
        }

        if (move.is_king_castling_ || move.is_queen_castling_) {
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
            std::pair<Position, bool &> l[] = {
                    {Position(File::A, Rank::ONE),   did_white_queen_castling_},
                    {Position(File::H, Rank::ONE),   did_white_king_castling_},
                    {Position(File::A, Rank::EIGHT), did_black_queen_castling_},
                    {Position(File::H, Rank::EIGHT), did_black_king_castling_},
            };
            for (auto association : l) {
                if (association.first == rook_position) {
                    association.second = true;
                }
            }
            move_piece((*this)[rook_position].value(), rook_destination);
        }

        //if the board previously had a 'en passant target square', it is now consume
        en_passant_target_square_.push_back(std::nullopt);

        if (move.is_double_pawn_push_) {
            //mark the square that can be "prise en passant"'ed next turn
            int direction = whose_turn_is_it() == Color::WHITE ? +1 : -1;
            en_passant_target_square_.push_back(std::make_optional<Position>(move.start_position_.file_get(),
                                                                             move.start_position_.rank_get() +
                                                                             direction));
        }

        if (move.promotion_.has_value()) {
            //move already happened so we use the endposition
            promote_piece((*this)[move.end_position_].value(), move.promotion_.value());
        }


        is_white_turn_ = !is_white_turn_;
    }


    void Chessboard::add_piece(const std::optional<Piece> p) {
        assert(p.has_value() && "last_piece_capture has no value");
        pieces_.push_back(p.value());
        (*this)[p->position_] = p;
        turns_since_last_piece_taken_or_pawn_moved_.pop_back();
    }

    void Chessboard::move_piece(const Piece &p, Position new_position) {
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
            turns_since_last_piece_taken_or_pawn_moved_.push_back(0);
        }

        //make sure this did what we want
        assert((*this)[new_position] == *piece_it);
        assert((*this)[new_position]->type_ == p.type_);
        assert((*this)[new_position]->position_ != p.position_);
        assert((*this)[new_position]->color_ == p.color_);
    }

    void Chessboard::promote_piece(const Piece &p, PieceType new_type) {
        assert(std::find(pieces_.begin(), pieces_.end(), p) != pieces_.end() && "queried piece not in piece list");
        auto piece_it = std::find(pieces_.begin(), pieces_.end(), p);
        piece_it->type_ = new_type;
        (*this)[piece_it->position_] = std::make_optional<Piece>(*piece_it);

        assert(std::find(pieces_.begin(), pieces_.end(), p)->type_ == new_type &&
               "promotion did not take effect in piece list");
        assert((*this)[p.position_]->type_ == new_type && "promotion did not take effect in piece list");
        assert(*std::find(pieces_.begin(), pieces_.end(), p) == (*this)[p.position_].value());
    }

    void Chessboard::remove_piece(const Piece &p) {
        assert(std::find(pieces_.begin(), pieces_.end(), p) != pieces_.end() && "queried piece not in piece list");
        auto piece_it = std::find(pieces_.begin(), pieces_.end(), p);
        pieces_.erase(piece_it);
        last_piece_capture.push_back((*this)[p.position_].value());
        (*this)[p.position_] = std::nullopt;
        turns_since_last_piece_taken_or_pawn_moved_.push_back(0);

        //make sure this did what we want
        assert(std::find(pieces_.begin(), pieces_.end(), p) == pieces_.end());
    }

}
