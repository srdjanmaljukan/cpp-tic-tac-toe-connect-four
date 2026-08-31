# Iks-oks & Connect 4 (C++)

Console-based C++ project featuring two classic games: Tic-Tac-Toe (Iks-oks) and Connect 4.

## Games

- **Tic-Tac-Toe** — classic 3x3 grid, two-player (or vs. simple AI).
- **Connect 4** — 7x6 grid, drop-token gravity mechanic, 4-in-a-row win check.

## Requirements

- C++17 compatible compiler (g++ / MSVC / clang)
- CMake (recommended for building)

## Build & Run

```bash
mkdir build && cd build
cmake ..
cmake --build .
./IksoksConnect4
```

## Status

🚧 Work in progress — Tic-Tac-Toe in development.

## Project Structure

```
src/
├── main.cpp
├── TicTacToe.h / .cpp
└── ConnectFour.h / .cpp
```