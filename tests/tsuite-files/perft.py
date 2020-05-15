
from chess import Board
from chess import Move
import json
import os
from os.path import isfile, join
import subprocess
import sys
import time
from termcolor import colored, cprint

def run_perft(path_chessengine, path_listener, path_perft_directory):
    files = [f for f in os.listdir(path_perft_directory) if isfile(join(path_perft_directory, f))]
    for file in files:
        run_perft_file(path_chessengine, path_listener, path_perft_directory + "/" + file)

def run_perft_file(path_chessengine, path_listener, path_perft):

    time_start = time.time()
    python_move_log = []
    chessengine_move_log = []
    def python_perft(depth, board) -> int:
        if depth == 1:
            for move in list(board.legal_moves):
                board_state = board.fen().split()[0]
                python_move_log.append(board_state + " " + move.uci())
            return len(list(board.legal_moves))
        res = 0
        for move in board.legal_moves:
            board.push(move)
            res += python_perft(depth - 1, board)
            board.pop()
        return res

    def chessengine_perft() -> int:
        chessengine_run = subprocess.run(["./" + path_chessengine, "--listeners", "./" + path_listener, "--perft", path_perft, "--debug"], stdout=subprocess.PIPE)
        l = chessengine_run.stdout.decode('utf-8').split('\n')
        for i in range(len(l) - 2):
            chessengine_move_log.append(l[i])
        #print("last: ", l[len(l) - 2])
        return int(len(l) - 2)

    with open(path_perft) as f:
        perft_line = str.strip(f.readline())
        fen, depth = ' '.join(perft_line.split()[:-1]), perft_line.split()[-1]
        a = python_perft(int(depth), Board(fen))
        b = chessengine_perft()

        if a != b:
            print(path_perft + ": python and engine did not find the same amount of moves. Python got " + str(a) + " whereas engine got " + str(b))

            def results_to_dict(results) -> dict:
                dict = {}
                for entry in results:
                    board = entry.split()[0]
                    move = entry.split()[1]
                    moves = dict.get(board)
                    if moves:
                        moves.append(move)
                        dict[board] = moves
                    else:
                        dict[board] = [move]
                return dict

            python_boards_moves = results_to_dict(python_move_log)
            chessengine_boards_moves = results_to_dict(chessengine_move_log)

            #print("python", python_boards_moves)
            #print("engine", chessengine_boards_moves)

            #print("python keys", python_boards_moves.keys())
            #print("engine keys", chessengine_boards_moves.keys())

            for board in python_boards_moves.keys():
                if chessengine_boards_moves.get(board):
                    python_moves = python_boards_moves.get(board)
                    chessengine_moves = chessengine_boards_moves.get(board)
                    for move in python_moves:
                        if move not in chessengine_moves:
                            print(board + " python got move: " + move + " whereas engine did not")
                    for move in chessengine_moves:
                        if move not in python_moves:
                            print(board + " engine got move: " + move + " whereas python did not")
            cprint(path_perft + " failure...", "orange")
        else:
            time_end = time.time()
            cprint(path_perft + " python and chess engine got the same perft results. Hurray ! computation took: " + str(time_end - time_start) + " seconds", "green")
        #print("-- chessengine output", chessengine_move_log, "--")
        #print("-- python output", python_move_log, "--")

