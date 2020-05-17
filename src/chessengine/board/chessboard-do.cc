//
// Created by xarang on 15/05/2020.
//

#include "chessboard.hh"

namespace board {


    void Chessboard::do_move(Move& move, bool change_turn) {

        current_turn_ += 1;
        //past_moves_halfmove_clocks_[past_moves_halfmove_clocks_.size() - 1]+=1;
        if (move.is_halfmove_clock_resetter()) {
            past_moves_halfmove_clocks_.push_back(0);
        }
        else {
            auto temp = past_moves_halfmove_clocks_.back() + 1;
            past_moves_halfmove_clocks_.push_back(temp);
        }


        //handle capture (+ special behaviour for en-passant)
        if (move.is_capture_) {
            if (!move.is_en_passant_) {
                remove_piece((*this)[move.end_position_].value());
            } else {
                int direction = whose_turn_is_it() == Color::WHITE ? 1 : -1;
                auto target = past_moves_en_passant_target_squares_.back().value();
                remove_piece((*this)[Position(target.file_get(), target.rank_get() - direction)].value());
            }
        }

        //handle movement (+ special behaviour for en-passant)
        if (!move.is_en_passant_) {
            move_piece((*this)[move.start_position_].value(), move.end_position_);
        } else {
            move_piece((*this)[move.start_position_].value(), past_moves_en_passant_target_squares_.back().value());
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


        if (move.is_double_pawn_push_) {
            //mark the square that can be "prise en passant"'ed next turn
            int direction = whose_turn_is_it() == Color::WHITE ? +1 : -1;
            past_moves_en_passant_target_squares_.emplace_back(std::make_optional<Position>(move.start_position_.file_get(),
                                                                             move.start_position_.rank_get() +
                                                                             direction));
        }
        else {
            past_moves_en_passant_target_squares_.emplace_back(std::nullopt);
        }

        if (move.promotion_.has_value()) {
            //move already happened so we use the endposition
            promote_piece((*this)[move.end_position_].value(), move.promotion_.value());
        }

        if (change_turn) {
            this->change_turn();
        }
    }



    void Chessboard::move_piece(const Piece &p, Position new_position) {
        std::vector<Piece>& piece_set = pieces_[{p.type_, p.color_}];
        auto piece_it = std::find(piece_set.begin(), piece_set.end(), p);
        assert(piece_it != piece_set.end() && "queried piece not in piece list");
        piece_it->has_already_moved_ = true;
        piece_it->position_ = new_position;

        (*this)[p.position_] = std::nullopt;
        (*this)[piece_it->position_] = std::make_optional<Piece>(*piece_it);
        assert((*this)[piece_it->position_].value() == *piece_it && "copy constructor of piece did not copy properly");


        //make sure this did what we want
        assert((*this)[new_position] == *piece_it);
        assert((*this)[new_position]->type_ == p.type_);
        assert((*this)[new_position]->position_ != p.position_);
        assert((*this)[new_position]->color_ == p.color_);
    }

    void Chessboard::promote_piece(const Piece &p, PieceType new_type) {
        std::vector<Piece>& piece_set = pieces_[{p.type_, p.color_}];
        auto piece_it = std::find(piece_set.begin(), piece_set.end(), p);
        assert(piece_it != piece_set.end() && "queried piece not in piece list");

        Piece promoted(*piece_it); //copy
        piece_set.erase(piece_it);

        promoted.type_ = new_type;
        std::vector<Piece>& destination_set = pieces_[{new_type, p.color_}];
        destination_set.emplace_back(promoted);
        (*this)[promoted.position_] = std::make_optional<Piece>(promoted);

        assert(std::find(piece_set.begin(), piece_set.end(), p) == piece_set.end() &&
               "promotion did not remove piece from its original piece set");
        assert(std::find(destination_set.begin(), destination_set.end(), promoted) != destination_set.end() &&
                "promotion did not add promoted piece to the proper set");
    }

    void Chessboard::remove_piece(const Piece &p) {
        //std::cerr << "remove piece: " << p.to_string() << "\n";
        std::vector<Piece>& piece_set = pieces_[{p.type_, p.color_}];
        auto piece_it = std::find(piece_set.begin(), piece_set.end(), p);
        if (piece_it == piece_set.end()) {
            std::cerr << p.to_string() << "\n";
        }
        assert(piece_it != piece_set.end() && "queried piece not in piece list");
        piece_set.erase(piece_it);
        last_pieces_captured_.push_back((*this)[p.position_].value());
        (*this)[p.position_] = std::nullopt;


        //make sure this did what we want
        assert(std::find(piece_set.begin(), piece_set.end(), p) == piece_set.end());
    }

}
