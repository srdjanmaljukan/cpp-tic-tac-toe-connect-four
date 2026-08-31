#include "ConnectFour.h"
#include <iostream>
#include <limits>
#include <algorithm>

ConnectFour::ConnectFour() {
    resetBoard();
    currentPlayer = 'X';
    vsComputer = false;
    humanSymbol = 'X';
    computerSymbol = 'O';
}

void ConnectFour::resetBoard() {
    for (auto& red : board) {
        red.fill(' ');
    }
}

void ConnectFour::printBoard() const {
    std::cout << "\n";
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            std::cout << "| " << board[r][c] << " ";
        }
        std::cout << "|\n";
    }
    for (int c = 1; c <= COLS; ++c) {
        std::cout << "  " << c << " ";
    }
    std::cout << "\n\n";
}

void ConnectFour::chooseMode() {
    int choice;
    while (true) {
        std::cout << "=== CONNECT 4 ===\n";
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

bool ConnectFour::isColumnFull(int col) const {
    return board[0][col] != ' ';
}

// Ubacuje zeton u kolonu date table (radi i nad pravom tablom i nad simulacijama)
int ConnectFour::dropToken(std::array<std::array<char, COLS>, ROWS>& b, int col, char player) {
    for (int r = ROWS - 1; r >= 0; --r) {
        if (b[r][col] == ' ') {
            b[r][col] = player;
            return r;
        }
    }
    return -1;
}

int ConnectFour::getPlayerInput() const {
    int input;
    while (true) {
        std::cout << "Igrac " << currentPlayer << ", izaberi kolonu (1-" << COLS << "): ";
        std::cin >> input;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Nevazeci unos, pokusaj ponovo.\n";
            continue;
        }

        if (input < 1 || input > COLS) {
            std::cout << "Kolona mora biti izmedju 1 i " << COLS << ".\n";
            continue;
        }

        int col = input - 1;
        if (isColumnFull(col)) {
            std::cout << "Ta kolona je puna, izaberi drugu.\n";
            continue;
        }

        return col;
    }
}

bool ConnectFour::checkWin(const std::array<std::array<char, COLS>, ROWS>& b, char player) const {
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c <= COLS - 4; ++c) {
            if (b[r][c] == player && b[r][c + 1] == player &&
                b[r][c + 2] == player && b[r][c + 3] == player) {
                return true;
            }
        }
    }

    for (int c = 0; c < COLS; ++c) {
        for (int r = 0; r <= ROWS - 4; ++r) {
            if (b[r][c] == player && b[r + 1][c] == player &&
                b[r + 2][c] == player && b[r + 3][c] == player) {
                return true;
            }
        }
    }

    for (int r = 0; r <= ROWS - 4; ++r) {
        for (int c = 0; c <= COLS - 4; ++c) {
            if (b[r][c] == player && b[r + 1][c + 1] == player &&
                b[r + 2][c + 2] == player && b[r + 3][c + 3] == player) {
                return true;
            }
        }
    }

    for (int r = 3; r < ROWS; ++r) {
        for (int c = 0; c <= COLS - 4; ++c) {
            if (b[r][c] == player && b[r - 1][c + 1] == player &&
                b[r - 2][c + 2] == player && b[r - 3][c + 3] == player) {
                return true;
            }
        }
    }

    return false;
}

bool ConnectFour::isBoardFull(const std::array<std::array<char, COLS>, ROWS>& b) const {
    for (int c = 0; c < COLS; ++c) {
        if (b[0][c] == ' ') return false;
    }
    return true;
}

void ConnectFour::switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

// Boduje jedan "prozor" od 4 polja za racunara (computerSymbol)
int ConnectFour::evaluateWindow(const std::array<char, 4>& window) const {
    int score = 0;
    int computerCount = 0, humanCount = 0, emptyCount = 0;

    for (char cell : window) {
        if (cell == computerSymbol) computerCount++;
        else if (cell == humanSymbol) humanCount++;
        else emptyCount++;
    }

    if (computerCount == 4) score += 1000;
    else if (computerCount == 3 && emptyCount == 1) score += 10;
    else if (computerCount == 2 && emptyCount == 2) score += 2;

    if (humanCount == 3 && emptyCount == 1) score -= 80; // blokiraj protivnika kao prioritet
    else if (humanCount == 2 && emptyCount == 2) score -= 2;

    return score;
}

// Ukupna evaluacija pozicije za racunara (vise = bolje za racunara)
int ConnectFour::scorePosition(const std::array<std::array<char, COLS>, ROWS>& b) const {
    int score = 0;

    // Bonus za centralnu kolonu
    int centerCol = COLS / 2;
    int centerCount = 0;
    for (int r = 0; r < ROWS; ++r) {
        if (b[r][centerCol] == computerSymbol) centerCount++;
    }
    score += centerCount * 3;

    // Horizontalni prozori
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c <= COLS - 4; ++c) {
            std::array<char, 4> window = { b[r][c], b[r][c + 1], b[r][c + 2], b[r][c + 3] };
            score += evaluateWindow(window);
        }
    }

    // Vertikalni prozori
    for (int c = 0; c < COLS; ++c) {
        for (int r = 0; r <= ROWS - 4; ++r) {
            std::array<char, 4> window = { b[r][c], b[r + 1][c], b[r + 2][c], b[r + 3][c] };
            score += evaluateWindow(window);
        }
    }

    // Dijagonala gore-lijevo -> dole-desno
    for (int r = 0; r <= ROWS - 4; ++r) {
        for (int c = 0; c <= COLS - 4; ++c) {
            std::array<char, 4> window = { b[r][c], b[r + 1][c + 1], b[r + 2][c + 2], b[r + 3][c + 3] };
            score += evaluateWindow(window);
        }
    }

    // Dijagonala dole-lijevo -> gore-desno
    for (int r = 3; r < ROWS; ++r) {
        for (int c = 0; c <= COLS - 4; ++c) {
            std::array<char, 4> window = { b[r][c], b[r - 1][c + 1], b[r - 2][c + 2], b[r - 3][c + 3] };
            score += evaluateWindow(window);
        }
    }

    return score;
}

// Minimax sa ogranicenom dubinom i alfa-beta odsijecanjem.
// bestCol se popunjava samo na pocetnom (najvisem) pozivu - koristimo ga da izvucemo potez, ne samo skor.
int ConnectFour::minimax(std::array<std::array<char, COLS>, ROWS>& b, int depth, int alpha, int beta, bool isMaximizing, int& bestCol) {
    bool computerWins = checkWin(b, computerSymbol);
    bool humanWins = checkWin(b, humanSymbol);
    bool full = isBoardFull(b);

    if (depth == 0 || computerWins || humanWins || full) {
        if (computerWins) return 1000000 - depth; // brza pobjeda je bolja
        if (humanWins) return depth - 1000000;    // sto kasnije da izgubi, to bolje
        if (full) return 0;
        return scorePosition(b);
    }

    // Koje kolone su dostupne (nisu pune)
    std::array<int, COLS> validCols;
    int validCount = 0;
    for (int c = 0; c < COLS; ++c) {
        if (b[0][c] == ' ') validCols[validCount++] = c;
    }

    if (isMaximizing) {
        int best = std::numeric_limits<int>::min();
        int localBestCol = validCols[0];

        for (int i = 0; i < validCount; ++i) {
            int c = validCols[i];
            int dummy;
            dropToken(b, c, computerSymbol);
            int score = minimax(b, depth - 1, alpha, beta, false, dummy);
            // vracamo kolonu u prethodno stanje (uklanjamo zeton)
            for (int r = 0; r < ROWS; ++r) {
                if (b[r][c] == computerSymbol) { b[r][c] = ' '; break; }
            }

            if (score > best) {
                best = score;
                localBestCol = c;
            }
            alpha = std::max(alpha, best);
            if (beta <= alpha) break;
        }

        bestCol = localBestCol;
        return best;
    }
    else {
        int best = std::numeric_limits<int>::max();
        int localBestCol = validCols[0];

        for (int i = 0; i < validCount; ++i) {
            int c = validCols[i];
            int dummy;
            dropToken(b, c, humanSymbol);
            int score = minimax(b, depth - 1, alpha, beta, true, dummy);
            for (int r = 0; r < ROWS; ++r) {
                if (b[r][c] == humanSymbol) { b[r][c] = ' '; break; }
            }

            if (score < best) {
                best = score;
                localBestCol = c;
            }
            beta = std::min(beta, best);
            if (beta <= alpha) break;
        }

        bestCol = localBestCol;
        return best;
    }
}

int ConnectFour::getBestMove() {
    std::array<std::array<char, COLS>, ROWS> temp = board;
    int bestCol = -1;
    minimax(temp, SEARCH_DEPTH, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), true, bestCol);
    return bestCol;
}

void ConnectFour::play() {
    resetBoard();
    currentPlayer = 'X';
    chooseMode();

    bool gameOver = false;
    printBoard();

    while (!gameOver) {
        int col;

        if (vsComputer && currentPlayer == computerSymbol) {
            std::cout << "Racunar razmislja...\n";
            col = getBestMove();
        }
        else {
            col = getPlayerInput();
        }

        dropToken(board, col, currentPlayer);
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
        else if (isBoardFull(board)) {
            std::cout << "Nerijeseno!\n";
            gameOver = true;
        }
        else {
            switchPlayer();
        }
    }
}