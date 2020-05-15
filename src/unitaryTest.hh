#pragma once
#include <random>
#include <cstring>

#include "chessengine/board/chessboard.hh"
#include "chessengine/board/chessboard-interface-impl.hh"
#include "listener-manager.hh"
#include "listener.hh"
#include "pgn/pgn-parser.hh"
#include "pgn/pgn-exception.hh"

namespace board {
    class UnitaryTest {
    public:
        UnitaryTest() = default;
        //tests COntroller
        int Controller();

    private:
        //test for undo_move
        int utest_undo_move();
        //test for ...
        std::string generateRNDBoard( int brds ) {
            std::string result = "";
            int a, b, i; char c;
            for( int cc = 0; cc < brds; cc++ ) {
                memset(brd, 0, 64 );
                std::string pieces = "PPPPPPPPNNBBRRQKppppppppnnbbrrqk";
                shuffle( pieces.begin(), pieces.end() , std::mt19937(std::random_device()()));

                while( pieces.length() ) {
                    i = rand() % pieces.length(); c = pieces.at( i );
                    while( true ) {
                        a = rand() % 8; b = rand() % 8;
                        if( brd[a][b] == 0 ) {
                            if((c == 'P' && !b) || (c == 'p' && b == 7) ||
                                ( ( c == 'K' || c == 'k' ) && search( c == 'k' ? 'K' : 'k', a, b ) ) ) continue;
                            break;
                        }
                    }
                    brd[a][b] = c;
                    pieces = pieces.substr( 0, i ) + pieces.substr( i + 1 );
                }
            }
            int e = 0;
            for( int y = 0; y < 8; y++ ) {
                for( int x = 0; x < 8; x++ ) {
                    if( brd[x][y] == 0 ) e++;
                    else {
                        if( e > 0 ) { result += std::to_string(e); e = 0; }
                            result += brd[x][y];
                    }
                }
                if( e > 0 ) { result += std::to_string(e); e = 0; }
                if( y < 7 ) result += "/";
            }
            result += " w - - 0 1";
            return result;
        }

    private:
        bool search( char c, int a, int b ) {
            for( int y = -1; y < 2; y++ ) {
                for( int x = -1; x < 2; x++ ) {
                    if( !x && !y ) continue;
                    if( a + x > -1 && a + x < 8 && b + y >-1 && b + y < 8 ) {
                        if( brd[a + x][b + y] == c ) return true;
                    }
                }
            }
            return false;
        }
        char brd[8][8];
        /*std::string fenArray[] = {"1R6/PpP3Q1/4P3/K3bP1p/pp1Np2p/Bprq1PP1/bPP1pknn/1RN1rB2 w - - 0 1",
                                  "6n1/n3rr1P/b1P1p1Q1/2Kp1pNp/kP2PP2/pp1q1pb1/NR2PpPP/2R1BB2 w - - 0 1",
                                  "5Q2/PP2P2k/3pK2p/2BP1p1b/rRpp1NPp/2PrpnR1/1PP1BbNp/1n3q2 w - - 0 1"};*/
    };
}