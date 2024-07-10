#include "Display.h"
#include "Sudoku.h"

int main() {
  int defaultBoardSize = 4;

  Sudoku sudoku(defaultBoardSize);
  Display display(sudoku);

  display.printBoard();

  return 0;
}