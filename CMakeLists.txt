# OPTIMISATION FLAGS : FIXME

set(OPT_FLAGS)

# END OPTIMIZATION FLAGS


# SETUP
cmake_minimum_required(VERSION 3.12)
project(Chess)

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")
# -fsanitize=address")

# LIBRARIES
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost REQUIRED COMPONENTS system program_options)
set(LIBRARIES Boost::system Boost::program_options ${CMAKE_DL_LIBS})


# SOURCES : FIXME

set(SRC
    src/chessengine/board/chessboard-interface-impl.cc
    src/chessengine/board/chessboard.cc
    src/chessengine/board/color.cc
    src/chessengine/board/move-builder.cc
    src/chessengine/board/move-legality-checker.cc
    src/chessengine/board/move.cc
    src/chessengine/board/piece.cc
    src/chessengine/board/position.cc
    src/chessengine/pgn/pgn-exception.cc
    src/chessengine/pgn/pgn-move.cc
    src/chessengine/pgn/pgn-parser.cc
    src/listeners/listener-manager.cc
        )

set (SRC_AI
        src/ai/uci.cc
        src/ai/minimax.cc
        )

include_directories(
    src/
    src/chessengine
    src/ai
    src/listeners
)

# END SOURCES

# AI

add_library(ai
        ${SRC_AI})


# CUSTOM LISTENER

add_library(best-listener-2022 SHARED
    src/custom-listeners/best-listener-2022.cc
    ${SRC})

add_library(listener-test-suite SHARED
        src/custom-listeners/listener-test-suite.cc
        ${SRC})

set_target_properties(best-listener-2022
    PROPERTIES
    EXCLUDE_FROM_ALL "true")

set_target_properties(listener-test-suite
        PROPERTIES
        EXCLUDE_FROM_ALL "true")

add_custom_target(listener DEPENDS best-listener-2022)
add_custom_target(listener-test DEPENDS listener-test-suite)



# CHESSENGINE
add_executable(chessengine)
target_sources(chessengine PRIVATE "src/main.cc" ${SRC})
target_link_libraries(chessengine PRIVATE ${LIBRARIES} ai)


# TESTS
# Check if GTest is installed
