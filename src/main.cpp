#include "TicTacToe.h"
#include "ConnectFour.h"
#include <iostream>
#include <limits>

int main() {
    int choice;

    while (true) {
        std::cout << "=== GLAVNI MENI ===\n";
        std::cout << "1. Iks-oks\n";
        std::cout << "2. Connect 4\n";
        std::cout << "0. Izlaz\n";
        std::cout << "Izbor: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Nevazeci unos.\n";
            continue;
        }

        if (choice == 1) {
            TicTacToe game;
            game.play();
        }
        else if (choice == 2) {
            ConnectFour game;
            game.play();
        }
        else if (choice == 0) {
            break;
        }
        else {
            std::cout << "Unesi 0, 1 ili 2.\n";
        }
    }

    return 0;
}