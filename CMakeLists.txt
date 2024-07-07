cmake_minimum_required(VERSION 3.10)
project(Sudoku VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add the executable
add_executable(Sudoku
    src/main.cpp
)

target_include_directories(Sudoku PRIVATE include)
target_link_libraries(Sudoku PRIVATE sfml-graphics sfml-window sfml-system)
