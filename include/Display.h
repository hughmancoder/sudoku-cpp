#ifndef DISPLAY_H
#define DISPLAY_H

#include "Sudoku.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Display {
  int CELL_SIZE = 100;
  int BUTTON_WIDTH = 200;
  int BUTTON_HEIGHT = 50;
  int WINDOW_WIDTH = CELL_SIZE * 4;
  int WINDOW_HEIGHT = CELL_SIZE * 4 + BUTTON_HEIGHT;
  float ASPECT_RATIO = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;

public:
  void printBoard(Board &board) {
    // Set the window to be fixed
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                            "4x4 Sudoku",
                            sf::Style::Titlebar | sf::Style::Close);

    sf::Font font;
    if (!font.loadFromFile("../resources/arial.ttf")) {
      std::cerr << "Error loading font" << std::endl;
      return;
    }

    // Create bottom buttons
    sf::RectangleShape validateButton(
        sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    validateButton.setPosition(0, 4 * CELL_SIZE);
    validateButton.setOutlineColor(sf::Color::Black);
    validateButton.setOutlineThickness(2);

    sf::Text validateText("Validate Board", font, 20);
    validateText.setFillColor(sf::Color::Black);
    validateText.setPosition(0.25 * CELL_SIZE, 4.15 * CELL_SIZE);

    sf::RectangleShape clearButton(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    clearButton.setPosition(2 * CELL_SIZE, 4 * CELL_SIZE);
    clearButton.setOutlineColor(sf::Color::Black);
    clearButton.setOutlineThickness(2);

    sf::Text clearText("Clear Board", font, 20);
    clearText.setFillColor(sf::Color::Black);
    clearText.setPosition(2.5 * CELL_SIZE, 4.15 * CELL_SIZE);

    bool showInvalid = false;
    std::vector<std::pair<int, int>> invalidCells;

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }
        handleMouseClick(event, board, validateButton, clearButton, showInvalid,
                         invalidCells);
      }

      window.clear();
      drawBoard(window, board, font, showInvalid, invalidCells);
      window.draw(validateButton);
      window.draw(validateText);
      window.draw(clearButton);
      window.draw(clearText);
      window.display();
    }
  }

private:
  void handleMouseClick(sf::Event &event, Board &board,
                        sf::RectangleShape &validateButton,
                        sf::RectangleShape &clearButton, bool &showInvalid,
                        std::vector<std::pair<int, int>> &invalidCells) {
    if (event.type == sf::Event::MouseButtonPressed) {
      int x = event.mouseButton.x / CELL_SIZE;
      int y = event.mouseButton.y / CELL_SIZE;

      // Check if click is within board bounds
      if (x >= 0 && x < board.size() && y >= 0 && y < board.size()) {
        board[y][x] = (board[y][x] % 4) + 1;
        showInvalid = false; // Reset validation on board change
      }

      if (validateButton.getGlobalBounds().contains(event.mouseButton.x,
                                                    event.mouseButton.y)) {
        showInvalid = true;
        invalidCells.clear();
        validateBoard(board, invalidCells);
      }

      if (clearButton.getGlobalBounds().contains(event.mouseButton.x,
                                                 event.mouseButton.y)) {
        clearBoard(board);
        showInvalid = false;
        invalidCells.clear();
      }
    }
  }

  void drawBoard(sf::RenderWindow &window, const Board &board,
                 const sf::Font &font, bool showInvalid,
                 const std::vector<std::pair<int, int>> &invalidCells) {
    for (int row = 0; row < board.size(); ++row) {
      for (int col = 0; col < board[row].size(); ++col) {
        sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        cell.setPosition(col * CELL_SIZE, row * CELL_SIZE);
        cell.setOutlineThickness(1);
        cell.setOutlineColor(sf::Color::Black);
        window.draw(cell);

        if (board[row][col] != 0) {
          sf::Text text;
          text.setFont(font);
          text.setString(std::to_string(board[row][col]));
          text.setCharacterSize(48);
          text.setFillColor(sf::Color::Black);
          text.setPosition(col * CELL_SIZE + 35, row * CELL_SIZE + 25);

          // Check if the cell is invalid
          if (showInvalid &&
              std::find(invalidCells.begin(), invalidCells.end(),
                        std::make_pair(row, col)) != invalidCells.end()) {
            text.setFillColor(sf::Color::Red);
          }

          window.draw(text);
        }
      }
    }
  }

  // TODO: implement validation algorithm
  void validateBoard(const Board &board,
                     std::vector<std::pair<int, int>> &invalidCells) {
    int size = board.size();
    for (int row = 0; row < size; ++row) {
      for (int col = 0; col < size; ++col) {
        if (!isValidNumber(board, row, col)) {
          invalidCells.emplace_back(row, col);
        }
      }
    }
  }

  // TODO: fix this function
  bool isValidNumber(const Board &board, int row, int col) {
    int size = board.size();
    int num = board[row][col];
    if (num == 0)
      return true;

    // Check row
    for (int i = 0; i < size; ++i) {
      if (i != col && board[row][i] == num)
        return false;
    }

    // Check column
    for (int i = 0; i < size; ++i) {
      if (i != row && board[i][col] == num)
        return false;
    }

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

  // TODO: replace with generate new sudoku board
  void clearBoard(Board &board) {
    for (auto &row : board) {
      std::fill(row.begin(), row.end(), 0);
    }
  }
};

#endif
