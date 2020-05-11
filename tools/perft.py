#!/usr/bin/env python3

from chess import Board
from chess import Move
import sys
import json


def perft(depth, board):
    if depth == 1:
        with open('pythonchess_perft_output.out', 'w') as f:
            for move in list(board.legal_moves):
                f.write(move.uci() + "\n")
        return len(list(board.legal_moves))

    res = 0
    for move in board.legal_moves:
        board.push(move)
        res += perft(depth - 1, board)
        board.pop()

    return res


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print(f"usage: {sys.argv[0]} [PERFT_FILE]")
        sys.exit(2)
    with open(sys.argv[1]) as f:
        perft_line = str.strip(f.readline())
        fen, depth = ' '.join(perft_line.split()[:-1]), perft_line.split()[-1]
        board = Board(fen)
    print(perft(int(depth), board))
