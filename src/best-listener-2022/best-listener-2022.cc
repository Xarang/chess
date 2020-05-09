#include "best-listener-2022.hh"

static std::string backgroundColor(int i, int j) {
    if ((i + j) % 2 == 0)
        return "40;";
    else
        return "47;";
}

std::string BestListener2022::board_string_representation() {
        //std::string vertical =   "| | | | | | | | |\n";
        std::string res = "";

        
        board::Rank currRank = board::Rank::EIGHT; 
        std::string background = "";

        res += "   A  B  C  D  E  F  G  H \n";
        for (int i = 7; i >= 0; i--)
        {
            board::File currFile = board::File::A;
            res += std::to_string(i + 1) + " ";
            for (int j = 0; j < 8; j++)
            {
                board::Position myPos(currFile, currRank);
                
                std::string charColor = "30m";
                char type = ' '; //what displays when empty
                if ((*interface_)[myPos].has_value()) {
                    auto myPiece = (*interface_)[myPos].value();
                    type = board::piece_to_char(myPiece.first);
                    auto color = myPiece.second;
                    if (color == board::Color::WHITE) {
                        charColor = "34m";
                    }
                    else {
                        charColor = "31m";
                    }
                }
                
                res += "\033[1;" + backgroundColor(i, j) + charColor + " " + type + " " + "\033[0m";
                currFile = currFile + 1;
            }
            res += " " + std::to_string(i + 1);
            res += "\n";
            currRank = currRank - 1;
        }
                res += "   A  B  C  D  E  F  G  H \n";
        return res;
    }