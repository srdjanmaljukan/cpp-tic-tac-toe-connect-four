#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <array>

class TicTacToe {
public:
    TicTacToe();

    // Pokreće glavnu petlju igre
    void play();

private:
    // Tabla 3x3, čuvamo kao niz od 9 polja ('X', 'O', ili ' ' za prazno)
    std::array<char, 9> board;

    // Trenutni igrač na potezu ('X' ili 'O')
    char currentPlayer;

    // Da li se igra protiv racunara, i koje slovo je racunar
    bool vsComputer;
    char humanSymbol;
    char computerSymbol;

    void printBoard() const;
    bool makeMove(int position);
    bool checkWin(const std::array<char, 9>& b, char player) const;
    bool isBoardFull() const;
    void switchPlayer();
    int getPlayerInput() const;
    void resetBoard();
    void chooseMode();

    // Minimax AI
    int minimax(std::array<char, 9>& b, int depth, bool isMaximizing, int alpha, int beta) const;
    int getBestMove() const;
};

#endif // TICTACTOE_H