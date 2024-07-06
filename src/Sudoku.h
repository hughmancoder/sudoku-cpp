#ifndef SUDOKU_H
#define SUDOKU_H

#include <string>
#include <vector>

using Board = std::vector<std::vector<int>>;

Board generateSudoku(int difficulty);

#endif