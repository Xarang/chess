
#include <iostream>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

#include "chessengine/chessengine.hh"
#include "chessengine/board/chessboard.hh"
#include "chessengine/board/chessboard-interface-impl.hh"
#include "listener-manager.hh"
#include "listener.hh"

int main(int argc, const char *argv[]) {

    namespace po = boost::program_options;
    auto options = po::options_description("Usage");

    options.add_options()
        ("help,h", "displays helpful message")
        ("pgn", po::value<std::string>(), "runs provided PGN on our chessengine")
        ("listeners,l", po::value<std::vector<std::string>>()->multitoken(), "plugs in provided listeners")
        ("perft", po::value<std::string>(), "runs provided Perft on our chessengine and output the amount of moves we were able to generate.")
        ;
    
    po::variables_map variables;
    po::store(po::parse_command_line(argc, argv, options), variables);
    po::notify(variables);

    if (variables.count("help")) {
        std::cout << options << "\n";
        return 1;
    }

    listener::ListenerManager listenerManager;
    if (variables.count("listeners")) {
        auto listeners = variables["listeners"].as<std::vector<std::string>>();
        listenerManager = listener::ListenerManager(listeners);
    }

    auto board = board::Chessboard();
    listenerManager.register_board(board);

    if (variables.count("pgn")) {
         //pgn here
         std::string pgnfilename = argv[1];
         try {
             chessengine::runPgnFile(board, pgnfilename);
         }
         catch (std::exception &e) {
             std::cerr << e.what() << "\n";
         }
    }
    else if (variables.count("perft")) {
         //generator here
         auto moves = board.generateLegalMoves();
         for (auto move : moves) {
             std::cout << move.to_string();
         }
    }
    else {
        //ai here
    }

    listenerManager.close_listeners();
    return 0;
}
