#ifndef DISPLAY_H
#define DISPLAY_H

#include "Sudoku.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

class Display {
  int CELL_SIZE = 100;
  int windowHeight, windowWidth, boardSize, buttonWidth, buttonHeight,
      buttonYPosition;
  Coord invalidCoordinates;
  Coord initialNonZeroCellCoordinates;
  Sudoku &sudoku;

public:
  Display(Sudoku &sudoku) : sudoku(sudoku) {
    updateBoardSize(sudoku.getBoardSize());
  }

  void updateBoardSize(int newBoardSize) {
    boardSize = newBoardSize;
    buttonHeight = CELL_SIZE / 2;
    windowHeight = CELL_SIZE * boardSize + buttonHeight;
    windowWidth = CELL_SIZE * boardSize;
    buttonYPosition = CELL_SIZE * boardSize;
    buttonWidth = windowWidth / 2;
  }

  void printBoard() {
    // Set the window to be fixed
    std::string titleMessage =
        std::to_string(boardSize) + "x" + std::to_string(boardSize) + " Sudoku";
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight),
                            titleMessage,
                            sf::Style::Titlebar | sf::Style::Close);

    sf::Font font;
    if (!font.loadFromFile("../resources/arial.ttf")) {
      std::cerr << "Error loading font" << std::endl;
      return;
    }

    // Create bottom buttons
    sf::RectangleShape validateButton(sf::Vector2f(buttonWidth, buttonHeight));
    validateButton.setPosition(0, buttonYPosition);
    validateButton.setOutlineColor(sf::Color::Black);
    validateButton.setOutlineThickness(2);

    sf::Text validateText("Check Solution", font, 20);
    validateText.setFillColor(sf::Color::Black);
    sf::FloatRect validateTextBounds = validateText.getLocalBounds();
    validateText.setOrigin(
        validateTextBounds.left + validateTextBounds.width / 2.0f,
        validateTextBounds.top + validateTextBounds.height / 2.0f);
    validateText.setPosition(validateButton.getPosition().x + buttonWidth / 2,
                             validateButton.getPosition().y + buttonHeight / 2);

    sf::RectangleShape clearButton(sf::Vector2f(buttonWidth, buttonHeight));
    clearButton.setPosition(buttonWidth, buttonYPosition);
    clearButton.setOutlineColor(sf::Color::Black);
    clearButton.setOutlineThickness(2);

    sf::Text clearText("New Board", font, 20);
    clearText.setFillColor(sf::Color::Black);
    sf::FloatRect clearTextBounds = clearText.getLocalBounds();
    clearText.setOrigin(clearTextBounds.left + clearTextBounds.width / 2.0f,
                        clearTextBounds.top + clearTextBounds.height / 2.0f);
    clearText.setPosition(clearButton.getPosition().x + buttonWidth / 2,
                          clearButton.getPosition().y + buttonHeight / 2);

    bool showInvalid = false;
    bool boardSolved = false;
    Coord initialNonZeroCellCoordinates =
        sudoku.getInitialNonZeroCellCoordinates();

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }
        handleMouseClick(event, validateButton, clearButton, showInvalid,
                         boardSolved);
      }

      window.clear();
      drawBoard(window, font, showInvalid, boardSolved,
                initialNonZeroCellCoordinates);
      window.draw(validateButton);
      window.draw(validateText);
      window.draw(clearButton);
      window.draw(clearText);
      window.display();
    }
  }

private:
  void handleMouseClick(sf::Event &event, sf::RectangleShape &validateButton,
                        sf::RectangleShape &clearButton, bool &showInvalid,
                        bool &boardSolved) {
    if (event.type == sf::Event::MouseButtonPressed) {
      int x = event.mouseButton.x / CELL_SIZE;
      int y = event.mouseButton.y / CELL_SIZE;

      if (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
        sudoku.updateCell(y, x);
        showInvalid = false;
        invalidCoordinates.clear();
      }

      if (validateButton.getGlobalBounds().contains(event.mouseButton.x,
                                                    event.mouseButton.y)) {
        showInvalid = true;
        invalidCoordinates = sudoku.getPrecomptedInvalidCoordinates();
        boardSolved = sudoku.isFull() && invalidCoordinates.empty();
      }

      if (clearButton.getGlobalBounds().contains(event.mouseButton.x,
                                                 event.mouseButton.y)) {
        sudoku.generateSudoku();
        showInvalid = false;
        initialNonZeroCellCoordinates =
            sudoku.getInitialNonZeroCellCoordinates();
        boardSolved = false;
      }
    }
  }

  void drawBoard(sf::RenderWindow &window, const sf::Font &font,
                 bool showInvalid, bool boardSolved,
                 Coord &initialNonZeroCellCoordinates) {
    const auto &board = sudoku.getCurrentBoard();

    for (int row = 0; row < boardSize; ++row) {
      for (int col = 0; col < boardSize; ++col) {
        sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        cell.setPosition(col * CELL_SIZE, row * CELL_SIZE);
        cell.setOutlineThickness(1);
        cell.setOutlineColor(sf::Color::Black);
        cell.setFillColor(sf::Color::White);
        window.draw(cell);

        if (board[row][col] != 0) {
          sf::Text text;
          text.setFont(font);
          text.setString(std::to_string(board[row][col]));
          text.setCharacterSize(48);

          sf::FloatRect textRect = text.getLocalBounds();
          text.setOrigin(textRect.left + textRect.width / 2.0f,
                         textRect.top + textRect.height / 2.0f);
          text.setPosition(col * CELL_SIZE + CELL_SIZE / 2.0f,
                           row * CELL_SIZE + CELL_SIZE / 2.0f);

          if (showInvalid &&
              std::find(invalidCoordinates.begin(), invalidCoordinates.end(),
                        std::make_pair(row, col)) != invalidCoordinates.end()) {
            text.setFillColor(sf::Color::Red);
          } else if (boardSolved) {
            text.setFillColor(sf::Color::Green);
          } else if (std::find(initialNonZeroCellCoordinates.begin(),
                               initialNonZeroCellCoordinates.end(),
                               std::make_pair(row, col)) !=
                     initialNonZeroCellCoordinates.end()) {
            text.setFillColor(sf::Color::Black);
          } else {
            text.setFillColor(sf::Color(100, 100, 100)); // Dark Gray
          }

          window.draw(text);
        }
      }
    }
  }

  void showPopup(sf::RenderWindow &window, const sf::Font &font,
                 const std::string &message) {
    sf::RectangleShape popup(
        sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 4.0f));
    popup.setFillColor(sf::Color::White);
    popup.setOutlineColor(sf::Color::Black);
    popup.setOutlineThickness(2);
    popup.setPosition(window.getSize().x / 4.0f, window.getSize().y / 3.0f);

    sf::Text text(message, font, 24);
    text.setFillColor(sf::Color::Black);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(popup.getPosition().x + popup.getSize().x / 2.0f,
                     popup.getPosition().y + popup.getSize().y / 2.0f);

    window.draw(popup);
    window.draw(text);
  }
};

#endif