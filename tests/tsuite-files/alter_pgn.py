import chess.pgn
import chess

import sys

def alter_pgn(pgn_file):
    pgn = open(pgn_file)
    game = chess.pgn.read_game(pgn)

    moves = game.mainline_moves()
    print(type(moves))


alter_pgn(sys.argv[1])