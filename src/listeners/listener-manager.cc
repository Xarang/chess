#include <dlfcn.h>
#include <iostream>
#include <string>

#include "../chessengine/pgn/pgn-parser.hh"

#include "listener-manager.hh"

namespace listener {

    ListenerManager::ListenerManager(::board::Chessboard& board, std::vector<std::string> plugins)
        : board_(board), interface_(board::ChessboardInterfaceImpl(board_)) {
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
            lst->register_board(interface_);
            listeners_.push_back(lst);
            plugins_.push_back(lib);
            std::cout << "[LOAD] plugin " << listener << " ready to go!" << "(" << plugins_.size() << "/" << plugins.size() << ")\n";
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

     void ListenerManager::runPgnFile(std::string& filename) {
        try {
            std::vector<board::PgnMove> moves = pgn_parser::parse_pgn(filename);
            for (board::PgnMove m : moves) {
                auto move = board::Move(m);
                std::cout << move.to_string();
                try {
                    if (board_.is_move_legal(move)) {
                        try {
                            board_.do_move(move);
                        }
                        catch (std::exception &e) {
                            throw std::runtime_error("error happened while executing move: " + move.to_string() + " : " + e.what() + "\n");
                        }

                    }
                    else {
                        throw std::logic_error("move was not legal.");
                    }
                }
                catch (std::logic_error &e) {
                    std::cerr << "Error happened while assessing legality of move: " + move.to_string() + "  : " << e.what() << "\n";
                    break;
                }
            }
        }
        catch (std::exception &e) {
            std::cerr << "Error while running PGN file: " << e.what() << "\n";
        }
    }



    //listener calls
    void ListenerManager::register_move(board::Color color, board::Move move, std::optional<board::Piece> destinationSquare) {
        for (auto listener : listeners_) {
            listener->on_piece_moved(move.piece_, move.start_position_, move.end_position_);
        }
        if (move.is_capture_) {
            //then destinationSquare has to hold something
            for (auto listener : listeners_) {
                listener->on_piece_taken(destinationSquare.value().type_, move.end_position_);
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
        }
    }

    void ListenerManager::register_game_draw() {
        for (auto listener : listeners_) {
            listener->on_draw();
        }
    }

    void ListenerManager::register_game_finished() {
        for (auto listener : listeners_) {
            listener->on_game_finished();
        }
    }


}
