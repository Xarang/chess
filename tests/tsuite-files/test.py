import argparse
import sys
from pgn import *
from perft import run_perft_file, run_perft
from termcolor import colored, cprint


global NumFile
def main(argv):

    parser = argparse.ArgumentParser()
    parser.add_argument("chessengine", help="binary to test")
    parser.add_argument("--pgn", help="pgn directory to process")
    parser.add_argument("--perft", help="perft directory to process")
    parser.add_argument("--all", help="pre built test suite recipe that tests a bit of everything", action="store_true")
    args = parser.parse_args()

    result = 0
    cprint("-----------------------TEST SUITE START--------------------------\n", "red")

    listener = "./liblistener-test-suite.so"
    if (args.pgn):
        result = test_pgn(args.chessengine, listener, args.pgn)

    if (args.perft):
        result = run_perft_file(args.chessengine, listener, args.perft)

    if (args.all):
        test_pgn(args.chessengine, listener, "data/pgn")
        run_perft(args.chessengine, listener, "data/perft/basic")

    cprint("\n---------------------TEST SUITE FINISH----------------------", "red")
    return result

if __name__ == "__main__":
    # execute only if run as a script
    main(sys.argv)