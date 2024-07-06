#include "Sudoku.h"
#include <fstream>
#include <iostream>
#include <random>

using namespace std;

const int SIZE = 4;

Board generateSudoku(int difficulty) {
  Board board(SIZE, vector<int>(SIZE, 0));
  // TODO: random generate
  board = {{1, 2, 3, 4}, {3, 4, 1, 2}, {4, 3, 2, 1}, {2, 1, 4, 3}};
  return board;
}

/* void saveState(const Board &board, const string &filename) {
  ofstream file(filename);
  if (file.is_open()) {
    for (const auto &row : board) {
      for (int num : row) {
        file << num << " ";
      }
      file << "\n";
    }
    file.close();
  } else {
    cout << "Unable to open file for saving.\n";
  }
}

Board loadState(const string &filename) {
  Board board(SIZE, vector<int>(SIZE, 0));
  ifstream file(filename);
  if (file.is_open()) {
    for (auto &row : board) {
      for (int &num : row) {
        file >> num;
      }
    }
    file.close();
  } else {
    cout << "Unable to open file for loading.\n";
  }
  return board;
} */