#include <dlfcn.h>
#include <iostream>
#include <fstream>
#include <string>

#include "ai/uci.hh"
#include "pgn/pgn-parser.hh"
#include "ai/minimax.hh"
#include "listener-manager.hh"

namespace listener {

    void ListenerManager::load_plugins(std::vector<std::string> plugins) {
        for (auto listener : plugins) {
            void *lib = dlopen(listener.c_str(), RTLD_LAZY);
            if (!lib) {
                std::cerr << "could not open listener lib " << listener << " : " << dlerror();
                throw std::runtime_error("could not open listener lib " + listener + " : " + dlerror());
            }
            void *listenerFunc = dlsym(lib, "listener_create");
            listener::Listener* lst = reinterpret_cast<listener::Listener*(*)()>(listenerFunc)();
            listeners_.push_back(lst);
            plugins_.push_back(lib);
        }
    }

    void ListenerManager::register_board(board::Chessboard& b) {
        board_ = std::make_optional<board::Chessboard>(b);
        all_boards_since_start_.insert(std::pair<std::string, int>(board_->to_string(), 1));
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

                            //3 fold rule check
                            auto fen = board_->to_string();
                            if (all_boards_since_start_.find(fen) != all_boards_since_start_.end()) {
                                all_boards_since_start_.insert_or_assign(fen, all_boards_since_start_.find(fen)->second + 1);
                                //std::cout << fen << " ---> " << all_boards_since_start_.find(fen)->second << "\n";
                            } else {
                                all_boards_since_start_.insert(std::pair<std::string, int>(fen, 1));
                            }
                            for (std::pair<std::string, int> pair : all_boards_since_start_) {
                                if (pair.second >= 3) {
                                    return register_game_draw();
                                }
                            }

                            //player 2
                            board_->change_turn();
                            //player 1
                            register_move(board_->whose_turn_is_it(), move, captured_piece); //do_move changes the player so we have to get the other player here (!whose_turn_is_it)   
                            board_->change_turn();
                            //player 2

                            if (board_->is_checkmate()) {
                                return register_mat(board_->whose_turn_is_it());
                            }
                            else {
                                if (board_->is_check()) {
                                    register_check(board_->whose_turn_is_it());
                                }
                                if (board_->generate_legal_moves().empty()) {
                                    return register_pat(board_->whose_turn_is_it());
                                }
                                else if (board_->is_draw()) {
                                    return register_game_draw();
                                }
                            }
                            //player 2
                        }
                        catch (std::exception &e) {
                            throw std::runtime_error("error happened while executing move: " + move.to_string() + " : " + e.what() + "\n");
                        }
                    }
                    else {
                        //std::cout << "[PGN RUNNER] move illegal: " + move.to_string();
                        return register_player_disqualification(board_->whose_turn_is_it());
                    }
                }
                catch (std::exception &e) {
                    std::cerr << "Error happened while assessing legality of move: " + move.to_string() + "  : " << e.what() << "\n";
                    return;
                }
            }
        }
        catch (std::bad_alloc &e /*std::exception &e we dont want to catch for now */) {
            std::cerr << "Error while running PGN file: " << e.what() << "\n";
        }
    }

    static unsigned long long perft(board::Chessboard b, int depth, bool debug) {
        if (depth == 0) {
            return 1;
        }
        unsigned long long sum = 0;
        auto moves = b.generate_legal_moves();
        for (auto move : moves) {
            if (depth == 1) {
                if (debug) {
                    std::cout << b.to_string() << " " << move.uci() << "\n";
                }
            }
            auto projection = b.project(move);
            sum += perft(projection, depth - 1, debug);
        }   
        return sum;
    }

    void ListenerManager::run_perft(int depth, bool debug) {
        auto n = perft(board_.value(), depth, debug);
        std::cout << n << "\n";
    }


    //TODO: figure this out
    void ListenerManager::run_ai() {
        ai::init("The best AI");

        ai::AI chess_ai;

        //open debug file at /.local/share/pychess/engine

        //erase debug content from previous run
        std::fstream f;
        f.open("chess_debug", std::ios::out | std::ios::trunc);
        f.close();
        while (true) { //end when ?

            //outputs received position string to log file
            std::string board_str = ai::get_board();
            f.open("chess_debug", std::ios::out | std::ios::app);
            f << "received board: " << board_str << std::endl;
            f.close();

            auto board = board::Chessboard::parse_uci(board_str);

            /*
            f.open("chess_debug", std::ios::out | std::ios::app);
            f << "playing : " << (board.whose_turn_is_it() == board::Color::WHITE ? "whites" : "blacks") << std::endl;
            f.close();
            */
            //ai get best move for board;
            //auto moves = board_->generate_legal_moves();

            std::string move;

            auto next_opening_move = chess_ai.get_next_opening_move(board.whose_turn_is_it());
            if (!next_opening_move.empty()) {
                move = next_opening_move;
            }
            else {
                move = chess_ai.searchMove(board).uci();
            }

            //outputs best move to log file
            f.open("chess_debug", std::ios::out | std::ios::app);
            f << "best move: " << move << std::endl;
            f.close();

            //send move
            ai::play_move(move);
        }
    }

    void ListenerManager::evaluate_ai() {
        auto moves = board_->generate_legal_moves();
        for (auto move : moves) {
            //TODO: show ai score for this move
            auto new_board = board_->project(move);
            ai::AI myAI;
            std::cout << move.uci() << " " << myAI.evaluate(new_board) << "\n";
        }
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
            listener->on_game_finished();
        }
    }

    void ListenerManager::register_pat(board::Color color) {
        for (auto listener : listeners_) {
            listener->on_player_pat(color);
            register_game_draw();
        }
    }

    void ListenerManager::register_game_draw() {
        for (auto listener : listeners_) {
            listener->on_draw();
            listener->on_game_finished();
        }
    }
    void ListenerManager::register_player_disqualification(board::Color color) {
        for (auto listener : listeners_) {
            listener->on_player_disqualified(color);
            listener->on_game_finished();
        }
    }
}
