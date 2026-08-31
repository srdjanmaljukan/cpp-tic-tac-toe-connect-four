#include "TicTacToe.h"
#include <iostream>
#include <limits>
#include <algorithm>

TicTacToe::TicTacToe() {
    resetBoard();
    currentPlayer = 'X';
    vsComputer = false;
    humanSymbol = 'X';
    computerSymbol = 'O';
}

void TicTacToe::resetBoard() {
    board.fill(' ');
}

void TicTacToe::printBoard() const {
    std::cout << "\n";
    for (int red = 0; red < 3; ++red) {
        std::cout << " " << board[red * 3] << " | " << board[red * 3 + 1]
            << " | " << board[red * 3 + 2] << " \n";
        if (red < 2) {
            std::cout << "---+---+---\n";
        }
    }
    std::cout << "\n";
}

void TicTacToe::chooseMode() {
    int choice;
    while (true) {
        std::cout << "=== IKS-OKS ===\n";
        std::cout << "1. Igrac vs Igrac\n";
        std::cout << "2. Igrac vs Racunar (minimax)\n";
        std::cout << "Izbor: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Nevazeci unos.\n";
            continue;
        }

        if (choice == 1) {
            vsComputer = false;
            break;
        }
        else if (choice == 2) {
            vsComputer = true;
            break;
        }
        else {
            std::cout << "Unesi 1 ili 2.\n";
        }
    }
}

int TicTacToe::getPlayerInput() const {
    int input;
    while (true) {
        std::cout << "Igrac " << currentPlayer << ", unesi poziciju (1-9): ";
        std::cin >> input;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Nevazeci unos, pokusaj ponovo.\n";
            continue;
        }

        if (input < 1 || input > 9) {
            std::cout << "Pozicija mora biti izmedju 1 i 9.\n";
            continue;
        }

        if (board[input - 1] != ' ') {
            std::cout << "To polje je vec zauzeto!\n";
            continue;
        }

        return input - 1;
    }
}

bool TicTacToe::makeMove(int position) {
    if (board[position] != ' ') {
        return false;
    }
    board[position] = currentPlayer;
    return true;
}

bool TicTacToe::checkWin(const std::array<char, 9>& b, char player) const {
    const int winPatterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (const auto& pattern : winPatterns) {
        if (b[pattern[0]] == player &&
            b[pattern[1]] == player &&
            b[pattern[2]] == player) {
            return true;
        }
    }
    return false;
}

bool TicTacToe::isBoardFull() const {
    for (char cell : board) {
        if (cell == ' ') return false;
    }
    return true;
}

void TicTacToe::switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

// Minimax sa alfa-beta odsijecanjem.
// Racunar (computerSymbol) maksimizira rezultat, covjek (humanSymbol) minimizira.
int TicTacToe::minimax(std::array<char, 9>& b, int depth, bool isMaximizing, int alpha, int beta) const {
    if (checkWin(b, computerSymbol)) return 10 - depth;
    if (checkWin(b, humanSymbol)) return depth - 10;

    bool full = true;
    for (char cell : b) {
        if (cell == ' ') { full = false; break; }
    }
    if (full) return 0;

    if (isMaximizing) {
        int best = std::numeric_limits<int>::min();
        for (int i = 0; i < 9; ++i) {
            if (b[i] == ' ') {
                b[i] = computerSymbol;
                int score = minimax(b, depth + 1, false, alpha, beta);
                b[i] = ' ';
                best = std::max(best, score);
                alpha = std::max(alpha, best);
                if (beta <= alpha) break;
            }
        }
        return best;
    }
    else {
        int best = std::numeric_limits<int>::max();
        for (int i = 0; i < 9; ++i) {
            if (b[i] == ' ') {
                b[i] = humanSymbol;
                int score = minimax(b, depth + 1, true, alpha, beta);
                b[i] = ' ';
                best = std::min(best, score);
                beta = std::min(beta, best);
                if (beta <= alpha) break;
            }
        }
        return best;
    }
}

int TicTacToe::getBestMove() const {
    // checkWin/minimax rade nad kopijom table, pa je const ok uz mutable-friendly pristup
    std::array<char, 9> temp = board;
    int bestScore = std::numeric_limits<int>::min();
    int bestMove = -1;

    for (int i = 0; i < 9; ++i) {
        if (temp[i] == ' ') {
            temp[i] = computerSymbol;
            int score = minimax(temp, 0, false,
                std::numeric_limits<int>::min(),
                std::numeric_limits<int>::max());
            temp[i] = ' ';
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    return bestMove;
}

void TicTacToe::play() {
    resetBoard();
    currentPlayer = 'X';
    chooseMode();

    bool gameOver = false;
    printBoard();

    while (!gameOver) {
        int position;

        if (vsComputer && currentPlayer == computerSymbol) {
            std::cout << "Racunar razmislja...\n";
            position = getBestMove();
        }
        else {
            position = getPlayerInput();
        }

        makeMove(position);
        printBoard();

        if (checkWin(board, currentPlayer)) {
            if (vsComputer && currentPlayer == computerSymbol) {
                std::cout << "Racunar je pobijedio!\n";
            }
            else {
                std::cout << "Igrac " << currentPlayer << " je pobijedio!\n";
            }
            gameOver = true;
        }
        else if (isBoardFull()) {
            std::cout << "Nerijeseno!\n";
            gameOver = true;
        }
        else {
            switchPlayer();
        }
    }
}