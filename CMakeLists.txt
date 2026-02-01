cmake_minimum_required(VERSION 3.31)
project(BetterSuperTicTacToeMinMax)

set(CMAKE_CXX_STANDARD 20)



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


if(MSVC)
    target_compile_options(BetterSuperTicTacToeMinMax PRIVATE /O2 /Oi /Ot /GL /arch:AVX2 /D_CRT_SECURE_NO_WARNINGS)
    set_target_properties(BetterSuperTicTacToeMinMax PROPERTIES LINK_FLAGS "/LTCG")
else()
    if(CMAKE_BUILD_TYPE MATCHES "Release")
        target_compile_options(BetterSuperTicTacToeMinMax PRIVATE -O3 -march=native
                -flto -funroll-loops
                -ffast-math)
        target_link_options(BetterSuperTicTacToeMinMax PRIVATE
                -flto
                -O3
        )
    endif()
endif()
