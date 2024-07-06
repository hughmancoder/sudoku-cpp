cmake_minimum_required(VERSION 3.10)
project(SudokuProject VERSION 1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(Sudoku
    src/main.cpp
    src/Sudoku.cpp
    src/SudokuSFML.cpp
)

target_include_directories(Sudoku PRIVATE include)
target_link_libraries(Sudoku sfml-graphics sfml-window sfml-system)