//
// Created by xarang on 15/05/2020.
//

#include "chessboard.hh"

namespace board {


    void Chessboard::do_move(const Move& move, bool change_turn) {

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
                remove_piece(*((*this)[move.end_position_]));
            } else {
                int direction = whose_turn_is_it() == Color::WHITE ? 1 : -1;
                auto target = past_moves_en_passant_target_squares_.back().value();
                remove_piece(*(*this)[Position(target.file_get(), target.rank_get() - direction)]);
            }
        }

        //handle movement (+ special behaviour for en-passant)
        if (!move.is_en_passant_) {
            move_piece(*(*this)[move.start_position_], move.end_position_);
        } else {
            move_piece(*(*this)[move.start_position_], past_moves_en_passant_target_squares_.back().value());
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
            move_piece(*((*this)[rook_position]), rook_destination);
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
            promote_piece(*((*this)[move.end_position_]), move.promotion_.value());
        }

        if (change_turn) {
            this->change_turn();
        }
    }


    //TODO: change  const Piece &p to Piece* ?
    void Chessboard::move_piece(Piece* p, const Position& new_position) {

        *((*this)[p->position_]) = nullptr;
        p->has_already_moved_ = true;
        p->position_ = new_position;
        *((*this)[(p->position_)]) = p;

        //make sure this did what we want
        assert(read(new_position) == p);
    }

    void Chessboard::promote_piece(Piece* p, const PieceType& new_type) {
        std::vector<Piece*>& piece_set = pieces_[{p->type_, p->color_}];
        auto piece_it = std::find(piece_set.begin(), piece_set.end(), p);
        assert(piece_it != piece_set.end() && "queried piece not in piece list");
        piece_set.erase(piece_it);

        p->type_ = new_type;
        put_piece(p);

        std::vector<Piece*>& destination_set = pieces_[{p->type_, p->color_}];
        assert(std::find(piece_set.begin(), piece_set.end(), p) == piece_set.end() &&
               "promotion did not remove piece from its original piece set");
        assert(std::find(destination_set.begin(), destination_set.end(), p) != destination_set.end() &&
                "promotion did not add promoted piece to the proper set");
    }

    void Chessboard::remove_piece(Piece* p) {
        //std::cerr << "remove piece: " << p.to_string() << "\n";
        std::vector<Piece*>& piece_set = pieces_[{p->type_, p->color_}];
        auto piece_it = std::find(piece_set.begin(), piece_set.end(), p);

        assert(piece_it != piece_set.end() && "queried piece not in piece list");
        piece_set.erase(piece_it);
        last_pieces_captured_.emplace_back(p);
        *((*this)[p->position_]) = nullptr;

        //make sure this did what we want
        assert(std::find(piece_set.begin(), piece_set.end(), p) == piece_set.end());
    }

}
