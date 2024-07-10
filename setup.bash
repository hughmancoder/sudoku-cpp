#!/bin/bash

if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi


cd build


echo "Configuring project with CMake..."
cmake ..


echo "Building the project..."
cmake --build .


echo "Running the game..."
./Sudoku