import chess
import chess.pgn
import subprocess
import sys
import threading
import queue
import random


chessengine_path="./" + sys.argv[1]
otherengine_path="./" + sys.argv[2]

def initialise_engine(path):
    engine_process = subprocess.Popen([path], universal_newlines=True, bufsize=1, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    print("launched " + path)

    engine_process.stdin.write("uci\n")
    engine_process.stdin.write("isready\n")
    #engine_process.stdin.write("setoption name Skill Level value 2") #check if this worked later

    print("sent uci introduction")

    for stdout_line in iter(engine_process.stdout.readline, ""):
        if (stdout_line.strip() == "readyok"):
            break
        print("got line from process: " + stdout_line.strip())

    return engine_process


chessengine_process = initialise_engine(chessengine_path)
otherengine_process = initialise_engine(otherengine_path)


game = chess.pgn.Game()


# assign black/white
processes = [chessengine_process, otherengine_process]
random.shuffle(processes)
whites_process = processes[0]
blacks_process = processes[1]

whites_clock = 0
blacks_clock = 0

first_turn = True

game_node = None
def play_turn(player_process):
    global game_node
    n = game if game_node == None else game_node

    print("playing on board: " + n.board().fen())
    player_process.stdin.write("position" + " fen " + n.board().fen() + "\n")
    player_process.stdin.write("go depth 1\n")
    print("sent position to player")

    player_best_move = ""
    for stdout_line in iter(player_process.stdout.readline, ""):
        player_response = str(stdout_line)
        print("player response: " + stdout_line)
        if (player_response.split()[0] != "bestmove"):
            continue
        player_best_move = player_response.split()[-1]
        #print("best move :", player_best_move)
        break
    print("player sent his move: " + player_best_move)
    try:
        game_node = n.add_variation(chess.Move.from_uci(player_best_move))
    except:
        return "illegal"
    if game_node.board().is_checkmate():
        return "win"
    elif game_node.board().can_claim_draw():
        return "draw"
    return ""


#while game not over
game_state = ""
while True:
    #white turn
    game_state = play_turn(whites_process)
    if game_state != "":
        break
    game_state = play_turn(blacks_process)
    if game_state != "":
        break
    #black turn

print("game over with status: " + game_state)
print("result: " + game.headers["Result"])


for process in processes:
    process.kill()
