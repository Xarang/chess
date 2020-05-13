import io
import os
from os.path import isfile, join
from termcolor import colored, cprint
import io
import subprocess


import chess.pgn

def test_pgn(path_chessengine, path_listener, path_pgn):
    print(path_chessengine, path_listener, path_pgn)
    files = [f for f in os.listdir(path_pgn) if isfile(join(path_pgn, f))]

    def chessengine_run_pgn(pgn_file):
        convert = subprocess.run(["./pgn-extract", pgn_file], stdout=subprocess.PIPE, stderr=subprocess.DEVNULL) #convert pgn file for python chess
        converted_pgn = io.StringIO(convert.stdout.decode('utf-8'))
        game = chess.pgn.read_game(converted_pgn)
        if (game == None):
            return False #could not be parsed

        is_legal = True

        board = game.board()
        for move in game.mainline_moves():
            if (move not in board.legal_moves):
                is_legal = False
                break
            board.push(move)
        print(is_legal)
        #print(convert.stdout)
        chessengine_pgn_run = subprocess.run(['./' + path_chessengine, "--listeners", './' + path_listener, "--pgn", pgn_file], stdout=subprocess.PIPE)

        print(chessengine_pgn_run.stdout)
        if is_legal and chessengine_pgn_run.stdout.decode('utf-8') == "":
            return True
        if not is_legal and chessengine_pgn_run.stdout.decode('utf-8') != "":
            return True

        return False


        

    for file in files:
        run = chessengine_run_pgn(path_pgn + "/" + file)
        cprint(path_pgn + "/" + file, "green" if run == True else "red")

    return 0