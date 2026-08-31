cmake_minimum_required(VERSION 3.10)
project(IksoksConnect4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(IksoksConnect4
    src/main.cpp
    src/TicTacToe.cpp
    src/ConnectFour.cpp
 "src/ConnectFour.h" "src/ConnectFour.cpp")