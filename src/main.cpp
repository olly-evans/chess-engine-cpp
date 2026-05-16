#include <iostream>

#include "board.hpp"
#include "sfml_renderer.hpp"

int main() {
    // SFMLRenderer renderer(WINDOW_WIDTH, WINDOW_NAME);

    // renderer.init_renderer();

    // i dont want sf:: in relation to board liek at all.

    // renderer.get_main_window();

    // pass it in and leave main_window as is i think.
    // main_window.

    Board board = Board(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
    board.init();
    board.run(); // board.update();

    // renderer.render(board);
    return 0;
}