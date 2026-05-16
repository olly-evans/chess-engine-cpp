#include <iostream>

#include "board.hpp"
#include "sfml_renderer.hpp"

int main() {

    Board board;

    SFMLRenderer renderer(board, WINDOW_WIDTH);
    renderer.init_renderer();
    renderer.run();

    return 0;
}