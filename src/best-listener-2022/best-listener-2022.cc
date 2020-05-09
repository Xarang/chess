#include "best-listener-2022.hh"


std::string BestListener2022::board_string_representation() {
        std::string horizontal = "________________\n|";
        //std::string vertical =   "| | | | | | | | |\n";
        std::string res = "";

        
        board::File currFile = board::File::A;
        std::string background = "";

        for (int i = 0; i < 8; i++)
        {
            board::Rank currRank = board::Rank::ONE; 
            res += horizontal;

            for (int j = 0; j < 8; j++)
            {
                board::Position myPos(currFile, currRank);
                auto myPiece = (*interface_)[myPos].value();
                auto type = board::piece_to_char(myPiece.first);
                auto color = myPiece.second;

                if (i + j % 2 == 0)
                    background = "40;";
                else
                    background = "47;";

                std::string element = "";

                if (color == board::Color::WHITE)
                {
                    element = "\033[1;" + background + "34m" + type + "\0330m|";
                }
                else
                {
                    element = "\033[1;" + background + "31m" + type + "\0330m|";
                }
                res += element;
                currFile = currFile + 1;
            }
            res += "\n";
            currRank = currRank + 1;
        }
        return res;
    }