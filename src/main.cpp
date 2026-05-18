#include <iostream>

#include "board.hpp"
#include "sfml_app.hpp"

int main() {

    Board board;

    SFMLApp app(board, WINDOW_WIDTH);
    app.init_renderer();
    app.run();

    return 0;
}