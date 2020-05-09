#pragma once

#include <string>
#include <vector>

#include "../chessengine/board/chessboard.hh"
#include "../chessengine/board/chessboard-interface-impl.hh"
#include "../chessengine/board/move.hh"

#include "listener.hh"

namespace listener {
    class ListenerManager {
        private:
            std::vector<void*> plugins_;
            std::vector<Listener*> listeners_;

            std::optional<board::Chessboard> board_;
            std::optional<board::ChessboardInterfaceImpl> interface_;

        public:
            void close_listeners();
            void load_plugins(std::vector<std::string> plugins);
            void register_board(::board::Chessboard& board);
            ListenerManager() = default;

            //tasks
            void runPgnFile(std::string& filename);

            //send informations about what happened on the chessboard to all listeners
            void register_move(board::Color color, board::Move move, std::optional<board::Piece> destinationSquare);
            void register_check(board::Color color);
            void register_mat(board::Color color);
            void register_pat(board::Color color);
            void register_lose(board::Color color);
            void register_game_draw();
            void register_game_finished();
    };
}
