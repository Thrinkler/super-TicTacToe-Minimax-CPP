cmake_minimum_required(VERSION 3.31)
project(BetterSuperTicTacToeMinMax)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_BUILD_TYPE MATCHES "Release")
    add_compile_options(-O3 -march=native -flto)
    add_link_options(-O3 -march=native -flto)
endif()

set(GAME_SOURCES
        board/board.cpp
        board/board.h
        board/superBoard.cpp
        board/superBoard.h
        bots/minimax.cpp
        bots/minimax.h
)

add_executable(BetterSuperTicTacToeMinMax
        main.cpp
        ${GAME_SOURCES}
)


add_executable(run_tests
        tests/test_board.cpp
        ${GAME_SOURCES}
)

