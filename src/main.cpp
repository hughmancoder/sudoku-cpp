#include "Display.h"
#include "Sudoku.h"
#include <iostream>

int main() {
  Sudoku sudoku;
  Display display;
  Board board = sudoku.generateSudoku(4);

  display.printBoard(board);

  if (sudoku.isValidBoard(board)) {
    std::cout << "Valid Sudoku solution!" << std::endl;
  } else {
    std::cout << "Invalid Sudoku solution!" << std::endl;
  }

  return 0;
}
