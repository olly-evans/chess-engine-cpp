#include <iostream>

#include "board.hpp"

int main() {

    bool enable_debug = true;
    
    Board board = Board(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, enable_debug);
    board.init();
    board.run();
    return 0;
}