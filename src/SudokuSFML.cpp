#include "SudokuSFML.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using Board = std::vector<std::vector<int>>;
void displayBoardSFML(Board &board) {
  // void displayBoardSFML(std::vector<std::vector<int>> &board) {
  const int cellSize = 100;
  const int gridSize = board.size();
  const int windowSize = gridSize * cellSize;

  // Load the font
  /* Font font;
  if (!font.loadFromFile("resources/arial.ttf")) {
      std::cerr << "Error loading font\n";
      return;
  } */

  RenderWindow window(VideoMode(windowSize, windowSize), "Sudoku Board");

  int selectedRow = 0;
  int selectedCol = 0;

  // TODO: fix this
  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed)
        window.close();

      if (event.type == Event::KeyPressed) {
        if (event.key.code >= Keyboard::Num1 &&
            event.key.code <= Keyboard::Num9) {
          // Map key press to number
          int number = event.key.code - Keyboard::Num0;
          board[selectedRow][selectedCol] = number;
        } else if (event.key.code >= Keyboard::Numpad1 &&
                   event.key.code <= Keyboard::Numpad9) {
          // Map Numpad key press to number
          int number = event.key.code - Keyboard::Numpad0;
          board[selectedRow][selectedCol] = number;
        } else if (event.key.code == Keyboard::Left) {
          selectedCol = (selectedCol > 0) ? selectedCol - 1 : gridSize - 1;
        } else if (event.key.code == Keyboard::Right) {
          selectedCol = (selectedCol + 1) % gridSize;
        } else if (event.key.code == Keyboard::Up) {
          selectedRow = (selectedRow > 0) ? selectedRow - 1 : gridSize - 1;
        } else if (event.key.code == Keyboard::Down) {
          selectedRow = (selectedRow + 1) % gridSize;
        }
      }
    }

    window.clear(Color::White);

    // Draw the grid
    for (int i = 0; i <= gridSize; ++i) {
      // Vertical lines
      RectangleShape vLine(Vector2f(2, windowSize));
      vLine.setPosition(i * cellSize, 0);
      vLine.setFillColor(Color::Black);
      window.draw(vLine);

      // Horizontal lines
      RectangleShape hLine(Vector2f(windowSize, 2));
      hLine.setPosition(0, i * cellSize);
      hLine.setFillColor(Color::Black);
      window.draw(hLine);
    }

    RectangleShape highlight(Vector2f(cellSize, cellSize));
    highlight.setPosition(selectedCol * cellSize, selectedRow * cellSize);
    highlight.setFillColor(Color(200, 200, 200, 100));
    window.draw(highlight);

    for (int i = 0; i < gridSize; ++i) {
      for (int j = 0; j < gridSize; ++j) {
        if (board[i][j] != 0) {
          Text text;
          // text.setFont(font);
          text.setString(std::to_string(board[i][j]));
          text.setCharacterSize(48);
          text.setFillColor(Color::Black);
          text.setPosition(j * cellSize + cellSize / 3,
                           i * cellSize + cellSize / 6);
          window.draw(text);
        }
      }
    }

    window.display();
  }
}