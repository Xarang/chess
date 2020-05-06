#include <iostream>
#include <string>

#include "chessengine/chessengine.hh"

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        return 1;
    }
    std::string pgnfilename = argv[1];
    try {
        runPgnFile(pgnfilename);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
