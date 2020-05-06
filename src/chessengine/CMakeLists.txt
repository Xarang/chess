add_library(CHESS_ENGINE
    chessengine.cc)

add_subdirectory(ai)
add_subdirectory(board)

target_link_libraries(CHESS_ENGINE BOARD)
target_link_libraries(CHESS_ENGINE AI)