
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

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

    auto listenerManager = listener::ListenerManager();
    if (variables.count("listeners")) {
        listenerManager.load_plugins(variables["listeners"].as<std::vector<std::string>>());
    }

    if (variables.count("pgn")) {
        auto board = board::Chessboard();
        listenerManager.register_board(board);
        //pgn here
        std::string pgnfilename = variables["pgn"].as<std::string>();
        try {
            listenerManager.run_pgn_file(pgnfilename);
        }
        catch (std::exception &e) {
            std::cerr << e.what() << "\n";
        }
    }
    else if (variables.count("perft")) {
        //generator here
        std::ifstream ifs(variables["perft"].as<std::string>());
        std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        std::string depth;
        while (content.back() != ' ') {
            depth = depth + content.back();
            content.erase(content.size() - 1, 1);
        }
        content.erase(content.size() - 1, 1);

        auto board = board::Chessboard(content);
        listenerManager.register_board(board);
        listenerManager.run_perft(std::atoi(depth.c_str()));
    }
    else {
        //ai here
    }

    listenerManager.close_listeners();
    return 0;
}
