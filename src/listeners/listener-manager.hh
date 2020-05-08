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

            board::Chessboard board_;

        public:
            void close_listeners();
            ListenerManager(::board::Chessboard& board) : board_(board) {};
            ListenerManager(::board::Chessboard& board, std::vector<std::string> plugins);

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

            ListenerManager() = default;
    };
}
