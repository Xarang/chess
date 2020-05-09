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
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g3")
# -fsanitize=address")

# LIBRARIES
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system program_options)
set(LIBRARIES Boost::system Boost::program_options)


# SOURCES : FIXME

set(SRC_ENGINE_DIRECTORY src/chessengine)
set(CHESSENGINE_LIBRARY CHESSENGINE)
set(SRC_LISTENERS_DIRECTORY src/listeners)
set(LISTENERS_LIBRARY LISTENERS)

include_directories(${SRC_ENGINE_DIRECTORY})
add_subdirectory(${SRC_ENGINE_DIRECTORY})
include_directories(${SRC_LISTENERS_DIRECTORY})
add_subdirectory(${SRC_LISTENERS_DIRECTORY})

# END SOURCES

# CUSTOM LISTENER

add_library(best-listener-2022 SHARED
    src/best-listener-2022/best-listener-2022.cc)

target_link_libraries(best-listener-2022 PRIVATE ${CHESSENGINE_LIBRARY})

set_target_properties(best-listener-2022
    PROPERTIES
    EXCLUDE_FROM_ALL "true")

add_custom_target(listener
    DEPENDS best-listener-2022)


# CHESSENGINE
add_executable(chessengine
    src/main.cc)
target_link_libraries(chessengine PRIVATE ${LIBRARIES} ${CHESSENGINE_LIBRARY} ${LISTENERS_LIBRARY})