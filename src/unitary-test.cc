#include "unitary-test.hh"
namespace board
{
    int UnitaryTest::Controller()
    {
        return utest_undo_move();
    }
    int UnitaryTest::utest_undo_move() {
        for (int i = 0; i < 100; i++) {
            auto fen = generateRNDBoard((rand()%1000));
            Chessboard board(fen);
            auto legals_moves = board.generate_legal_moves(true);
            for (auto moves : legals_moves)
            {
                Chessboard copyBoard(fen);
                copyBoard.do_move(moves);
                copyBoard.undo_move(moves);
                if (!(board == copyBoard))
                {
                    std::cout << "\nFor the move " << moves.to_string() << " undo move failed " <<
                    board.to_string() << " " << copyBoard.to_string() << " \n";
                }
            }
        }
        for (auto fen : fen_array)
        {
            if (fen == "Fen strings here")
                break;
            Chessboard board(fen);
            auto legals_moves = board.generate_legal_moves(true);
            for (auto moves : legals_moves)
            {
                Chessboard copyBoard(fen);
                copyBoard.do_move(moves);
                copyBoard.undo_move(moves);
                if (!(board == copyBoard))
                {
                    std::cout << "\nFor the move " << moves.to_string() << " undo move failed " <<
                              board.to_string() << " " << copyBoard.to_string() << " \n";
                }
            }
        }
        return 0;
    }
}