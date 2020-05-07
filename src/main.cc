#include <iostream>
#include <string>

#include "chessengine/chessengine.hh"
#include "chessengine/board/chessboard.hh"

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        //generator here
        auto board = board::ChessboardImpl();
        auto moves = board.generateLegalMoves();
        for (auto move : moves) {
            std::cout << move.to_string();
        }
        return 1;
    }


    //pgn here
    std::string pgnfilename = argv[1];
    try {
        chessengine::runPgnFile(pgnfilename);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
