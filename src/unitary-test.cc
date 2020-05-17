#include "unitary-test.hh"

namespace board
{
    static int test_undo_castling() {
        std::cout << "Unit test: undo castling\n";
            auto board = Chessboard("2k5/8/8/8/8/8/8/R3K2R w KQ - 0 1");
            for (auto move : board.generate_legal_moves()) {
                if (move.is_king_castling_ || move.is_queen_castling_) {
                    auto cpy = board;
                    board.do_move(move);
                    board.undo_move(move);
                    if (!(cpy == board)) {
                        std::cout << "Undo castling failed\n";
                    }
                }
            }
        std::cout << "Unit test: undo castling test completed\n";
        return 0;
    }

    static int test_undo_en_passant() {
        std::cout << "Unit test: undo en passant\n";
        auto board = Chessboard("2k5/4p3/8/3P4/8/8/8/R3K2R b KQ - 0 1");
        auto black_pawn_double_push = board.parse_uci_move("e7e5");
        assert(black_pawn_double_push.is_double_pawn_push_ && "board did not recognise this as a double pawn push");
        auto cpy = board;
        board.do_move(black_pawn_double_push);
        auto white_prend_en_passant = board.parse_uci_move("d5e6");
        assert(white_prend_en_passant.is_en_passant_ && "board did not recognise this as a en passant");
        auto cpy_2 = board;
        board.do_move(white_prend_en_passant);
        board.undo_move(white_prend_en_passant);
        if (!(board == cpy_2)) {
            std::cout << "undo prise en passant failed\n";
        }
        board.undo_move(black_pawn_double_push);
        if (!(cpy == board)) {
            std::cout << "undo double pawn push failed\n";
        }
        std::cout << "Unit test: undo en passant test completed\n";
        return 0;
    }


    int UnitaryTest::utest_undo_move_rnd() {
        std::cout << "Unit test: RND test\n";
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
        std::cout << "Unit test: RND test completed\n";
        return 0;
    }

    int UnitaryTest::Controller()
    {
        test_undo_castling();
        test_undo_en_passant();
        utest_undo_move_rnd();
        return 0;
    }

}