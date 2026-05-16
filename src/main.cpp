#include <iostream>

#include "board.hpp"
#include "sfml_renderer.hpp"

int main() {

    SFMLRenderer renderer(WINDOW_WIDTH);
    renderer.init_renderer();

    // i dont want sf:: in relation to board liek at all.

    // renderer.get_main_window();

    // pass it in and leave main_window as is i think.
    // main_window.

    Board board = Board(renderer.get_board_square_size());
    board.init();
    board.run(); // board.update();

    // renderer.render(board);
    return 0;
}