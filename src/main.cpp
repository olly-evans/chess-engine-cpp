#include <iostream>

#include "board.hpp"
#include "player.hpp"

int main() {
    // SFMLRenderer renderer;
    // renderer.init_renderer();

    // i dont want sf:: in relation to board liek at all.

    // renderer.get_main_window();

    Board board = Board(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
    board.init();
    board.run(); // board.update();

    // renderer.render(board);
    return 0;
}