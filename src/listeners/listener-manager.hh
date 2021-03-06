#pragma once

#include <string>
#include <vector>

#include "board/chessboard.hh"
#include "board/chessboard-interface-impl.hh"
#include "board/move.hh"

#include "listener.hh"

namespace listener {
    class ListenerManager {
        private:
            std::vector<void*> plugins_;
            std::vector<Listener*> listeners_;

            std::shared_ptr<board::Chessboard> board_;
            std::optional<board::ChessboardInterfaceImpl> interface_;

            std::unordered_map<std::string, int> all_boards_since_start_ = std::unordered_map<std::string, int>();

        public:
            void close_listeners();
            void load_plugins(std::vector<std::string> plugins);
            void register_board(const std::shared_ptr<board::Chessboard>& board);
            ListenerManager() = default;

            //tasks
            void run_pgn_file(std::string& filename);
            void run_perft(int depth, bool debug=false);
            void run_ai();

            //send informations about what happened on the chessboard to all listeners
            void register_move(board::Color color, board::Move& move, std::optional<board::Piece> captured_piece);
            void register_check(board::Color color);
            void register_mat(board::Color color);
            void register_pat(board::Color color);
            void register_player_disqualification(board::Color color);
            void register_game_draw();
    };
}
