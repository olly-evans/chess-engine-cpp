#include <iostream>

#include "board.hpp"
#include "player.hpp"

int main() {
    Board board = Board(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
    board.init();
    board.run();
    return 0;
}