#pragma once

#include <string>
#include <vector>

#include "../chessengine/board/chessboard.hh"
#include "../chessengine/board/chessboard-interface-impl.hh"

#include "listener.hh"

namespace listener {
    class ListenerManager {
        private:
            std::vector<void*> plugins_;
            std::vector<Listener*> listeners_;

        public:
            void close_listeners();
            ListenerManager(std::vector<std::string> plugins);
            void register_board(::board::Chessboard& board);
            ListenerManager() = default;
    };
}
