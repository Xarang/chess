
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <boost/program_options.hpp>

#include "chessengine/board/chessboard.hh"
#include "chessengine/board/chessboard-interface-impl.hh"
#include "listener-manager.hh"
#include "listener.hh"
#include "pgn/pgn-parser.hh"
#include "pgn/pgn-exception.hh"
#include "unitary-test.hh"

//tracing related functions
#include <stdio.h>
#include <time.h>
#include <dlfcn.h>

static FILE *fp_trace;

void
__attribute__ ((constructor))
trace_begin (void)
{
    fp_trace = fopen("trace.out", "w");
}

void
__attribute__ ((destructor))
trace_end (void)
{
    if(fp_trace != NULL) {
        fclose(fp_trace);
    }
}


extern "C" {
#define __NON_INSTRUMENT_FUNCTION__    __attribute__((__no_instrument_function__))

    void
    __NON_INSTRUMENT_FUNCTION__
    __cyg_profile_func_enter(void *func, void *caller) {
        Dl_info info_func;
        Dl_info info_caller;
        if (fp_trace != NULL) {
            if (dladdr(func, &info_func) && dladdr(caller, &info_caller)) {
                fprintf(fp_trace, "e %s %lu\n",
                        info_func.dli_sname,
                        time(NULL));
            }
        }
    }

    void
    __NON_INSTRUMENT_FUNCTION__
    __cyg_profile_func_exit(void *func, void *caller) {
        Dl_info info_func;
        Dl_info info_caller;
        if (fp_trace != NULL) {
            if (dladdr(func, &info_func) && dladdr(caller, &info_caller)) {
                fprintf(fp_trace, "e %s %lu\n",
                        info_func.dli_sname,
                        time(NULL));
            }
        }
    }
}

////// main program starts here

//static attributes declarations for all classes that need it
std::unordered_map<char, std::vector<board::Position>> board::Chessboard::initial_positions;

int main(int argc, const char *argv[]) {
        
    assert(std::is_copy_constructible<board::Chessboard>::value);
    assert(std::is_copy_constructible_v<board::Chessboard>);

    namespace po = boost::program_options;
    auto options = po::options_description("Usage");
    options.add_options()
        ("help,h", "displays helpful message")
        ("pgn", po::value<std::string>(), "runs provided PGN on our chessengine")
        ("listeners,l", po::value<std::vector<std::string>>()->multitoken(), "plugs in provided listeners")
        ("evaluate", po::value<std::string>(), "evaluates moves for given fen-represented board.")
        ("perft", po::value<std::string>(), "runs provided Perft on our chessengine and output the amount of moves we were able to generate.")
        ("unit-test","run unitary-test undo move")
        ("debug", "activates debugging/testing output for some features")
        ;


    //static attributes initialisations for all classes that need it
    board::Chessboard::initialise_chessboard_static_attributes();

    po::variables_map variables;
    po::store(po::parse_command_line(argc, argv, options), variables);
    po::notify(variables);


    if (variables.count("help")) {
        std::cout << options << "\n";
        return 1;
    }
    if (variables.count("unit-test"))
    {
        auto UT = board::UnitaryTest();
        return UT.Controller();
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
        catch (pgn_parser::PgnParsingException &e) {
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
        listenerManager.run_perft(std::atoi(depth.c_str()), variables.count("debug"));
        
    }
    else if (variables.count("evaluate")) {
        auto str = (variables.find("evaluate"))->second.as<std::string>();
        auto board = board::Chessboard(str);
        listenerManager.register_board(board);
        listenerManager.evaluate_ai();
    }
    else {

        listenerManager.run_ai();
    }

    listenerManager.close_listeners();
    return 0;
}
