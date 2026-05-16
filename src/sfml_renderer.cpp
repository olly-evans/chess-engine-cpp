#include "sfml_renderer.hpp"

// perhaps takes in an SFMLEventHandler

SFMLRenderer::SFMLRenderer(const uint16_t w_width) :
    win_w(w_width), 
    main_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_WIDTH), WINDOW_NAME)
    {}

bool SFMLRenderer::is_square_black(uint8_t i) {
    uint8_t x = i % GRID_SZ;
    uint8_t y = i / GRID_SZ;
    return (x + y) % 2;
}

/* INIT */

void SFMLRenderer::init_renderer() {

    set_board_square_size(board_square_size);
    set_main_window_squares();
}

void SFMLRenderer::set_board_square_size(uint16_t& sz) {
    if ((win_w % GRID_SZ) != 0 | (win_w % GRID_SZ) != 0)    
        exit(1);

    sz = win_w / GRID_SZ;
}

void SFMLRenderer::set_main_window_squares() {

    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        sf::Vector2f normalised_pos(i % GRID_SZ, i / GRID_SZ);
        sf::Vector2f pos = normalised_pos * (float)board_square_size;
        
        sf::RectangleShape rec(sf::Vector2f(board_square_size, board_square_size));

        rec.setPosition(pos);
        rec.setFillColor(is_square_black(i) ? MEDIUM_BROWN : WARM_CREAM);
        squares.insert(squares.begin(), rec);
    }
}

sf::RenderWindow& SFMLRenderer::get_main_window() {
    return main_window;
}

void SFMLRenderer::run() {
    
    while (main_window.isOpen()) {
        return;
    }
}