#include "Sudoku.h"
#include "SudokuSFML.h"
#include <iostream>

int main() {
  Board board = generateSudoku(1);
  displayBoardSFML(board);

  return 0;
}