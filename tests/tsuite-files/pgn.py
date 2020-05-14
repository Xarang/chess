import io
import os
from os.path import isfile, join
from termcolor import colored, cprint
import io
import subprocess

import chess.pgn
import time

import numpy as np

def test_pgn(path_chessengine, path_listener, path_pgn):
    print(path_chessengine, path_listener, path_pgn)


    files = [f for f in os.listdir(path_pgn) if isfile(join(path_pgn, f))]
    print("found ", len(files), "pgn files to process")

    timers = []

    def chessengine_run_pgn(pgn_file):

        time_start = time.time()
        chessengine_pgn_run = subprocess.run(['./' + path_chessengine, "--listeners", './' + path_listener, "--pgn", pgn_file], stdout=subprocess.PIPE)
        time_end = time.time()
        timers.append(time_end - time_start)

        convert = subprocess.run(["./pgn-extract", pgn_file], stdout=subprocess.PIPE, stderr=subprocess.DEVNULL) #convert pgn file for python chess
        converted_pgn = io.StringIO(convert.stdout.decode('utf-8'))
        game = chess.pgn.read_game(converted_pgn)
        if (game == None):
            return chessengine_pgn_run.stdout.decode('utf-8') != "" #could not be parsed (bad pgn or illegal move)

        is_legal = True

        board = game.board()
        for move in game.mainline_moves():
            if move not in board.legal_moves:
                is_legal = False
                break
            board.push(move)
        #print(is_legal)
        #print(convert.stdout)
        #print(chessengine_pgn_run.stdout)
        if is_legal and chessengine_pgn_run.stdout.decode('utf-8') == "":
            return True
        if not is_legal and chessengine_pgn_run.stdout.decode('utf-8') != "":
            return True

        return False


        

    for file in files:
        run = chessengine_run_pgn(path_pgn + "/" + file)
        #cprint(path_pgn + "/" + file, "green" if run == True else "red")
        if run == False:
            cprint(path_pgn + "/" + file, "red")


    timers = np.array(timers)

    print("done testing pgn files. Errors have been reported, if any.")
    print("average pgn computation time: ", timers.mean() )
    return 0