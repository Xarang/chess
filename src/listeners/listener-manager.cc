#include <dlfcn.h>
#include <iostream>
#include <string>

#include "pgn/pgn-parser.hh"

#include "listener-manager.hh"

namespace listener {

    void ListenerManager::load_plugins(std::vector<std::string> plugins) {
        for (auto listener : plugins) {
            void *lib = dlopen(listener.c_str(), RTLD_LAZY);
            if (!lib) {
                std::cerr << "could not open listener lib " << listener << " : " << dlerror();
                throw std::runtime_error("could not open listener lib " + listener + " : " + dlerror());
            }
            std::cout << "[LOAD] loaded listener lib: " << listener << "\n";
            void *listenerFunc = dlsym(lib, "listener_create");
            std::cout << "[LOAD] extracted 'listener_create' from lib: " << listenerFunc << "\n";
            listener::Listener* lst = reinterpret_cast<listener::Listener*(*)()>(listenerFunc)();
            listeners_.push_back(lst);
            plugins_.push_back(lib);
            std::cout << "[LOAD] plugin " << listener << " ready to go!" << "(" << plugins_.size() << "/" << plugins.size() << ")\n";
        }
    }

    void ListenerManager::register_board(board::Chessboard& b) {
        board_ = std::make_optional<board::Chessboard>(b);
        interface_ = std::make_optional<board::ChessboardInterfaceImpl>(board_.value());
        for (auto listener : listeners_) {
            listener->register_board(interface_.value());
        }
    }

    void ListenerManager::close_listeners() {
        for (auto listener: listeners_ )
        {
            delete(listener);
        }

        for (auto plugin : plugins_) {
            dlclose(plugin);
        }
    }

    //tasks

     void ListenerManager::run_pgn_file(std::string& filename) {
        try {
            std::vector<board::PgnMove> moves = pgn_parser::parse_pgn(filename);
            for (board::PgnMove m : moves) {
                auto move = board::Move(m);
                //std::cout << move.to_string();
                try {
                    if (board_->is_move_legal(move)) {
                        try {
                            std::optional<board::Piece> captured_piece;
                            if (move.is_capture_) {
                                if (move.is_en_passant_) {
                                    auto actual_piece_position = board::Position(move.end_position_.file_get(), move.end_position_.rank_get() + (board_->whose_turn_is_it() == board::Color::WHITE ? -1 : 1));
                                    captured_piece = std::make_optional<board::Piece>(((*board_)[actual_piece_position]).value());
                                } else {
                                    captured_piece = std::make_optional<board::Piece>(((*board_)[move.end_position_]).value());
                                }
                            }
                            //check current game state for the player that did the move
                            board_->do_move(move); //player 1
                            //player 2
                            board_->change_turn();
                            //player 1
                            register_move(board_->whose_turn_is_it(), move, captured_piece); //do_move changes the player so we have to get the other player here (!whose_turn_is_it)   
                            board_->change_turn();
                            //player 2

                            if (board_->is_checkmate()) {
                                register_mat(board_->whose_turn_is_it());
                                register_lose(board_->whose_turn_is_it());
                            }
                            else {
                                if (board_->is_check()) {
                                    register_check(board_->whose_turn_is_it());
                                }
                                if (board_->generateLegalMoves().size() == 0) {
                                    register_pat(board_->whose_turn_is_it());
                                    register_game_draw();
                                }
                                else if (board_->is_draw()) {
                                    register_game_draw();
                                }
                            }
                            //player 2
                        }
                        catch (std::bad_alloc &e /*std::exception &e we dont want to catch for now */) {
                            throw std::runtime_error("error happened while executing move: " + move.to_string() + " : " + e.what() + "\n");
                        }
                    }
                    else {
                        std::cout << "[PGN RUNNER] move illegal: " + move.to_string();
                        register_lose(board_->whose_turn_is_it());
                        return;
                    }
                }
                catch (std::bad_alloc &e /*we dont want to catch for now*/) {
                    std::cerr << "Error happened while assessing legality of move: " + move.to_string() + "  : " << e.what() << "\n";
                    break;
                }
            }
        }
        catch (std::bad_alloc &e /*std::exception &e we dont want to catch for now */) {
            std::cerr << "Error while running PGN file: " << e.what() << "\n";
        }
    }

    static unsigned long perft(board::Chessboard b, int depth) {
        if (depth == 0) {
            return 0;
        }
        unsigned long sum = 0;
        auto moves = b.generateLegalMoves();
        sum += moves.size();
        for (auto move : moves) {
            auto projection = b.project(move);
            sum += perft(projection, depth - 1);
        }
        return sum;
    }

    void ListenerManager::run_perft(int depth) {
        auto n = perft(board_.value(), depth);
        std::cout << n << "\n";
    }


    //listener calls
    void ListenerManager::register_move(board::Color color, board::Move move, std::optional<board::Piece> captured_piece) {
        for (auto listener : listeners_) {
            listener->on_piece_moved(move.piece_, move.start_position_, move.end_position_);
        }
        if (move.is_capture_) {
            //then destinationSquare has to hold something
            for (auto listener : listeners_) {
                listener->on_piece_taken(captured_piece.value().type_, move.end_position_);
            }
        }
        else if (move.is_king_castling_) {
            for (auto listener : listeners_) {
                listener->on_kingside_castling(color);
            }
        }
        else if (move.is_queen_castling_) {
            for (auto listener : listeners_) {
                listener->on_queenside_castling(color);
            }
        }

        if (move.promotion_.has_value()) {
            for (auto listener : listeners_) {
                listener->on_piece_promoted(move.promotion_.value(), move.end_position_);
            }
        }
    }

    void ListenerManager::register_check(board::Color color) {
        for (auto listener : listeners_) {
            listener->on_player_check(color);
        }
    }

    void ListenerManager::register_mat(board::Color color) {
        for (auto listener : listeners_) {
            listener->on_player_mat(color);
        }
    }

    void ListenerManager::register_pat(board::Color color) {
        for (auto listener : listeners_) {
            listener->on_player_pat(color);
        }
    }

    void ListenerManager::register_lose(board::Color color) {
        for (auto listener : listeners_) {
            listener->on_player_disqualified(color);
            listener->on_game_finished();
        }
    }

    void ListenerManager::register_game_draw() {
        for (auto listener : listeners_) {
            listener->on_draw();
            listener->on_game_finished();
        }
    }


}
