import chess
import subprocess
import sys
import threading
import queue


chessengine="./chessengine"
otherengine="./stockfish"

def initialise_engine(path):
    engine_process = subprocess.Popen([path], universal_newlines=True, bufsize=1, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    print("launched " + path)

    engine_process.stdin.write("uci\n")
    engine_process.stdin.write("isready\n")

    print("sent uci introduction")


    for stdout_line in iter(ent -mgine_process.stdout.readline, ""):
        if (stdout_line.strip() == "readyok"):
            break
        print("got line from process: " + stdout_line)


    return engine_process


chessengine_process = initialise_engine("./" + sys.argv[1])

chessengine_process.kill()

#otherengine_process = subprocess.Popen([otherengine_process])