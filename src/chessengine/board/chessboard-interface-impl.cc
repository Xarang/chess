#include "pgn/pgn-parser.hh"
#include "pgn/pgn-move.hh"
#include "chessboard-interface-impl.hh"
#include "piece-type.hh"

#include "position.hh"
#include "color.hh"

namespace board {
    using side_piece_t = std::pair<PieceType, Color>;
    using opt_piece_t = std::optional<side_piece_t>; 

    opt_piece_t ChessboardInterfaceImpl::operator[](const Position& position) const {
        std::optional<Piece> p = cb_[position];
        if (p.has_value()) {
            return std::pair<PieceType, Color>(p->type_, p->color_);
        }
        return std::nullopt;
    }

    std::string ChessboardInterfaceImpl::piece_to_char(PieceType myPiece) {
        if (myPiece == PieceType::KING)
            return "K";
        if (myPiece == PieceType::QUEEN)
            return "Q";
        if (myPiece == PieceType::BISHOP)
            return "B";
        if (myPiece == PieceType::ROOK)
            return "R";
        if (myPiece == PieceType::PAWN)
            return "P";  
        return "";  
    }

    std::string ChessboardInterfaceImpl::toString() {
        std::string horizontal = "________________\n|";
        //std::string vertical =   "| | | | | | | | |\n";
        std::string res = "";

        
        File currFile = File::A;
        std::string background = "";

        for (int i = 0; i < 8; i++)
        {
            Rank currRank = Rank::ONE; 
            res += horizontal;

            for (int j = 0; j < 8; j++)
            {
                Position myPos(currFile, currRank);
                auto myPiece = cb_[myPos].value();
                auto type = piece_to_char(myPiece.type_);

                if (i + j % 2 == 0)
                    background = "40;";
                else
                    background = "47;";

                std::string element = "";

                if (myPiece.color_ == Color::WHITE)
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

   

}