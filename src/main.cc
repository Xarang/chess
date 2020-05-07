
#include <dlfcn.h>
#include <iostream>
#include <string>

#include "chessengine/chessengine.hh"
#include "chessengine/board/chessboard.hh"
#include "chessengine/board/chessboard-interface-impl.hh"
#include "listener.hh"

int main(int argc, const char *argv[]) {

    void *lib = dlopen("./libbasic-output.so", RTLD_LAZY);
    if (!lib) {
        std::cerr << "could not open listener lib: " << dlerror();
        return 1;
    }
    std::cout << "[LISTENER] loaded listener lib: " << lib << "\n";
    void *listenerFunc = dlsym(lib, "listener_create");
    std::cout << "[LISTENER] extracted 'listener_create' from lib: " << listenerFunc << "\n";
    listener::Listener* lst = reinterpret_cast<listener::Listener*(*)()>(listenerFunc)();
    std::cout << "[LISTENER] reinterpreted listener; ready to go!" << "\n";

    if (argc != 2) {
        //generator here
        auto board = board::Chessboard();
        lst->register_board(board::ChessboardInterfaceImpl(board));
        auto moves = board.generateLegalMoves();
        for (auto move : moves) {
            std::cout << move.to_string();
        }
    }
    else {
        //pgn here
        std::string pgnfilename = argv[1];
        try {
            chessengine::runPgnFile(pgnfilename);
        }
        catch (std::exception &e) {
            std::cerr << e.what() << "\n";
            dlclose(lib);
            return 1;
        }
    }
    dlclose(lib);
    return 0;
   
}
