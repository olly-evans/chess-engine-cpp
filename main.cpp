#include <iostream>
#include "board.hpp"

int main() {

    // smfl_window_init();
    Board board = Board(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
    board.init();
    board.run();

    // Board board = Board();
    // board.init();
        // get_square_size();


    // sfml_window_init()
    

    


    //
    return 0;
    
}

/*
    - Colour an 8x8 grid in CLI.

    - Board class.
        - Piece class.
        - Print functions will be polymorphic I believe. Print differing char dependant on piece.
    - Each piece color has a bitboard.
    - Create a starting board.

    - Takes input from cin as move.
    

*/