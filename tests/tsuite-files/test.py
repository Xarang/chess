import argparse
import sys
from pgn import *
from perft import run_perft_file
from termcolor import colored, cprint


global NumFile
def main(argv):

    parser = argparse.ArgumentParser()
    parser.add_argument("chessengine", help="binary to test")
    parser.add_argument("--pgn", help="pgn directory to process")
    parser.add_argument("--perft", help="perft directory to process")
    args = parser.parse_args()

    result = 0
    cprint("-----------------------TEST SUITE START--------------------------\n", "red")

    if (args.pgn != None):
        result = test_pgn(args.chessengine, "./liblistener-test-suite.so", args.pgn)

    if (args.perft != None):
        result = run_perft_file(args.chessengine, "./liblistener-test-suite.so", args.perft)
        pass

    cprint("\n---------------------TEST SUITE FINISH----------------------", "red")
    return result

if __name__ == "__main__":
    # execute only if run as a script
    main(sys.argv)