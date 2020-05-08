add_library(CHESSENGINE
    chessengine.cc)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ai)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/board)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/pgn)

add_subdirectory(ai/)
add_subdirectory(board/)
add_subdirectory(pgn/)

target_link_libraries(CHESSENGINE PGN BOARD AI LISTENERS)