#include <iostream>

#include "chessboard.hh"
#include "pgn/pgn-move.hh"
#include "pgn/pgn-parser.hh"


namespace board 
{
    void runPgnFile(std::string& filename) {

        try {
            std::vector<PgnMove> moves = pgn_parser::parse_pgn(filename);
            auto chessboard = ListBasedChessboard();
            for (PgnMove m : moves) {
                auto move = Move(m);
                if (chessboard.is_move_legal(move)) {
                    chessboard.do_move(move);
                }
                else {
                    throw new std::runtime_error("move was not legal.");
                }
            }
        }
        catch (std::exception &e) {
            std::cerr << "Could not parse provided PGN file.\n" << e.what() << "\n";
        }

    }
}
