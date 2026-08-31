#ifndef CONNECTFOUR_H
#define CONNECTFOUR_H

#include <array>

const int ROWS = 6;
const int COLS = 7;
const int SEARCH_DEPTH = 6;

class ConnectFour {
public:
    ConnectFour();

    void play();

private:
    std::array<std::array<char, COLS>, ROWS> board;
    char currentPlayer;

    bool vsComputer;
    char humanSymbol;
    char computerSymbol;

    void printBoard() const;
    bool isColumnFull(int col) const;
    int dropToken(std::array<std::array<char, COLS>, ROWS>& b, int col, char player);
    bool checkWin(const std::array<std::array<char, COLS>, ROWS>& b, char player) const;
    bool isBoardFull(const std::array<std::array<char, COLS>, ROWS>& b) const;
    void switchPlayer();
    int getPlayerInput() const;
    void resetBoard();
    void chooseMode();

    // AI (minimax sa ogranicenom dubinom)
    int evaluateWindow(const std::array<char, 4>& window) const;
    int scorePosition(const std::array<std::array<char, COLS>, ROWS>& b) const;
    int minimax(std::array<std::array<char, COLS>, ROWS>& b, int depth, int alpha, int beta, bool isMaximizing, int& bestCol);
    int getBestMove();
};

#endif // CONNECTFOUR_H