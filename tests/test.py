from Player import *
from os import *
from os.path import isfile, join
import sys
import subprocess

path_pgn = "pgn/"

def test_pgn():
    fichiers = [f for f in listdir(path_pgn) if isfile(join(path_pgn, f))]
    for elt in fichiers:
        refWinner = findWinner(elt)
        proc = subprocess.Popen(['../build/chessengine', '--listener', '../build/libbest-listener-2022.so', '--pgn', '../pgn/' + elt], stdout=subprocess.PIPE)
        stdout_value = proc.communicate()[0]
        index = repr(stdout_value).find("[RESULT] ")
        substr = repr(stdout_value)[index:]
        end = substr.find("\n")
        result = repr(stdout_value)[index:end]
        print('\tstdout:', repr(stdout_value))
    return 0
def test_perft():
    print("Nt implemented")
    return 2

def main(*args):
    if (args[0][1] == "--pgn"):
        return test_pgn()
    if (args[0][1] == "--perft"):
        return test_perft()
    else:
        print("Not a option")
    return 0

if __name__ == "__main__":
    # execute only if run as a script
    main(sys.argv)
