class Player:
    def __init__(self, name):
        self.name = name


def findWinner(pgnFile):
    path = "pgn/"+pgnFile
    file = open(path)
    winner = ""
    b = False
    for ligne in file:
        index = ligne.find("Result")
        if index == -1:
            continue
        else:
            index = ligne.find(" ")
            index += 1
            winner = ligne[index + 1: len(ligne) - 3]
    if winner == "*":
        return Player("*")
    if winner == "1-0":
        return Player("White won")
    if winner == "0-1":
        return Player("Black won")
    return Player("Draw")