#ifndef SUDOKU_H
#define SUDOKU_H

#include <random>
#include <vector>

using Board = std::vector<std::vector<int>>;

class Sudoku {
public:
  Board generateSudoku(int size) {
    Board board(size, std::vector<int>(size, 0));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, size);

    for (int i = 0; i < size; ++i) {
      board[i][i] = dis(gen);
    }
    return board;
  }

  bool isValidBoard(const Board &board) {
    int size = board.size();
    for (int row = 0; row < size; ++row) {
      for (int col = 0; col < size; ++col) {
        if (!isValidNumber(board, row, col)) {
          return false;
        }
      }
    }
    return true;
  }

private:
  bool isValidNumber(const Board &board, int row, int col) {
    int size = board.size();
    int num = board[row][col];
    if (num == 0)
      return true;

    // Check the row
    for (int i = 0; i < size; ++i) {
      if (i != col && board[row][i] == num)
        return false;
    }

    // Check the column
    for (int i = 0; i < size; ++i) {
      if (i != row && board[i][col] == num)
        return false;
    }

    // Check the 2x2 sub-grid
    int subGridSize = std::sqrt(size);
    int subRowStart = (row / subGridSize) * subGridSize;
    int subColStart = (col / subGridSize) * subGridSize;

    for (int r = subRowStart; r < subRowStart + subGridSize; ++r) {
      for (int c = subColStart; c < subColStart + subGridSize; ++c) {
        if ((r != row || c != col) && board[r][c] == num)
          return false;
      }
    }

    return true;
  }
};

#endif // SUDOKU_H
