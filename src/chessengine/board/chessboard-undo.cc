//
// Created by xarang on 15/05/2020.
//

#include "chessboard.hh"

namespace board {

    void Chessboard::undo_move(Move& move, bool change_turn) {
        assert(!past_moves_halfmove_clocks_.empty());
        assert(!past_moves_en_passant_target_squares_.empty());

        current_turn_ -= 1;
        if (change_turn) {
            this->change_turn();
        }


        //pop back last entry in all turn-by-turn registers
        past_moves_halfmove_clocks_.pop_back();
        past_moves_en_passant_target_squares_.pop_back();

        //undo the move
        undo_move_piece(*read(move.end_position_), move.start_position_);

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
            undo_move_piece(*read(rook_position), rook_destination);
        }


        if (move.promotion_.has_value()) {
            //Check undo_promotion use start_position because this move is undo
            undo_promote_piece(*read(move.start_position_), move.piece_);
        }

        if (move.is_capture_) {
            //if (!move.is_en_passant_) {
            auto piece = last_pieces_captured_.back();
            undo_remove_piece(piece);
            last_pieces_captured_.pop_back();

        }

    }

    void Chessboard::undo_remove_piece(const Piece& p) {
        std::vector<Piece>& piece_set = pieces_[{p.type_, p.color_}];
        piece_set.push_back(p);
        *((*this)[p.position_]) = &(piece_set.back());
    }

    void Chessboard::undo_move_piece(const Piece &p, Position old_position) {
        std::vector<Piece>& piece_set = pieces_[{p.type_, p.color_}];
        auto piece_it = std::find(piece_set.begin(), piece_set.end(), p);
        assert(piece_it != piece_set.end() && "queried piece not in piece list");
        assert(*piece_it == p && "fetched piece is not equal to the queried piece");
        auto fen = p.to_char_fen();
        piece_it->has_already_moved_ = std::find(initial_positions[fen].begin(), initial_positions[fen].end(), old_position) == initial_positions[fen].end();
        piece_it->position_ = old_position;

        *((*this)[p.position_]) = nullptr;
        *((*this)[piece_it->position_]) = &(*piece_it);

        //make sure this did what we want
        assert(read(old_position) == &(*piece_it) && "did not undo move piece properly");
    }

    void Chessboard::undo_promote_piece(const Piece &p, PieceType old_type) {
        std::vector<Piece>& piece_set = pieces_[{p.type_, p.color_}];
        auto piece_it = std::find(piece_set.begin(), piece_set.end(), p);
        assert(piece_it != piece_set.end() && "queried piece not in piece list");

        Piece demoted(*piece_it); //copy
        piece_set.erase(piece_it);

        demoted.type_ = old_type;
        std::vector<Piece>& destination_set = pieces_[{old_type, p.color_}];
        destination_set.emplace_back(demoted);
        *((*this)[demoted.position_]) = &(destination_set.back());

        assert(std::find(piece_set.begin(), piece_set.end(), p) == piece_set.end() &&
               "demotion did not remove piece from its original piece set");
        assert(std::find(destination_set.begin(), destination_set.end(), demoted) != destination_set.end() &&
               "demotion did not add promoted piece to the proper set");
    }

}