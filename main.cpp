#include <iostream>

#include "board.hpp"
#include "pieces.hpp"

int main() {
    Board board = Board(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, true);
    board.init();
    board.run();
    return 0;
}

/*

    - Board class.
        - Piece class.
        - Print functions will be polymorphic I believe. Print differing char dependant on piece.
    - Each piece color has a bitboard.
    - Create a starting board.
*/