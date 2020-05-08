#include <iostream>

#include "board/chessboard.hh"
#include "../pgn/pgn-move.hh"
#include "../pgn/pgn-parser.hh"


namespace chessengine
{
    void runPgnFile(board::Chessboard& chessboard, std::string& filename) {
        try {
            std::vector<board::PgnMove> moves = pgn_parser::parse_pgn(filename);
            for (board::PgnMove m : moves) {
                auto move = board::Move(m);
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
