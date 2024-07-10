#ifndef SUDOKU_H
#define SUDOKU_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

using Board = std::vector<std::vector<int>>;
using Coord = std::vector<std::pair<int, int>>;

class Sudoku {
  Board board;
  Board solvedBoard;
  Coord initialNonZeroCellCoordinates;
  int size;

public:
  Sudoku(int size)
      : size(size), board(size, std::vector<int>(size, 0)),
        solvedBoard(size, std::vector<int>(size, 0)) {
    generateSudoku();
  }

  void generateSudoku() {
    clearBoard(); // Reset the board to an empty state

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<int> numbers(size);
    std::iota(numbers.begin(), numbers.end(), 1);
    std::shuffle(numbers.begin(), numbers.end(), gen);

    for (int i = 0; i < size; ++i) {
      board[i][i] = numbers[i];
    }

    solvedBoard = getSolvedBoard(board);
    board = solvedBoard;
    removeNumbers(board);
    initialNonZeroCellCoordinates = populateInitialNonZeroCellCoordinates();
  }

  Coord populateInitialNonZeroCellCoordinates() const {
    std::vector<std::pair<int, int>> nonZeroCoords;
    for (int row = 0; row < board.size(); ++row) {
      for (int col = 0; col < board[row].size(); ++col) {
        if (board[row][col] != 0) {
          nonZeroCoords.emplace_back(row, col);
        }
      }
    }
    return nonZeroCoords;
  }

  const Board &getCurrentBoard() const { return board; }

  bool isFull() const {
    for (const auto &row : board) {
      for (int cell : row) {
        if (cell == 0) {
          return false;
        }
      }
    }
    return true;
  }

  void updateCell(int row, int col) {
    board[row][col] = (board[row][col] % size) + 1;
  }

  bool isValidBoard() {
    for (int row = 0; row < size; ++row) {
      for (int col = 0; col < size; ++col) {
        if (!validateCell(board, row, col)) {
          return false;
        }
      }
    }
    return true;
  }

  Coord getPrecomptedInvalidCoordinates() {
    std::vector<std::pair<int, int>> invalidCells;
    for (int row = 0; row < size; ++row) {
      for (int col = 0; col < size; ++col) {
        if (board[row][col] != solvedBoard[row][col]) {
          invalidCells.emplace_back(row, col);
        }
      }
    }
    return invalidCells;
  }

  int getBoardSize() const { return size; }

  Coord getInitialNonZeroCellCoordinates() {
    return initialNonZeroCellCoordinates;
  }

private:
  void clearBoard() {
    for (auto &row : board) {
      std::fill(row.begin(), row.end(), 0);
    }
  }

  bool validateCell(Board &board, int row, int col) const {
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

    // Check the sub-grid (square)
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

  Board getSolvedBoard(Board board, int row = 0, int col = 0) {
    int size = board.size();

    if (row == size) {
      return board;
    }

    if (col == size) {
      return getSolvedBoard(board, row + 1, 0);
    }

    if (board[row][col] != 0) {
      return getSolvedBoard(board, row, col + 1);
    }

    for (int num = 1; num <= size; ++num) {
      board[row][col] = num;
      if (validateCell(board, row, col)) {
        Board result = getSolvedBoard(board, row, col + 1);
        if (!result.empty()) {
          return result;
        }
      }
      board[row][col] = 0;
    }
    // No Solution found
    return {};
  }

  void removeNumbers(Board &board, float fractionToRemove = 0.5) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, size - 1);

    int count = ((size * size) * std::fmin(fractionToRemove, 1));

    while (count > 0) {
      int row = dis(gen);
      int col = dis(gen);

      if (board[row][col] == 0)
        continue;

      board[row][col] = 0;
      --count;
    }
  }
};

#endif