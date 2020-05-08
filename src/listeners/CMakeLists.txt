add_library(LISTENERS
    listener-manager.cc)

target_link_libraries(LISTENERS PRIVATE CHESSENGINE ${CMAKE_DL_LIBS})