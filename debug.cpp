#include "debug.hpp"
#include "board.hpp"
#include <iostream>

void Debug::draw_cycle_bitboard(sf::RenderWindow &window, int win_w, int win_h, std::vector<uint64_t> bitboards, std::vector<std::string> bitboard_names, int bitboard_vec_index, std::vector<sf::RectangleShape> &squares) {
    
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        int draw_idx = GRID_NUM_SQUARES - i - 1;
        squares[draw_idx].setFillColor(bitboards[bitboard_vec_index] & (1ULL << i) ? sf::Color::White : sf::Color::Black);
    }

    // if (!window.isOpen()) {
    //     window.create(sf::VideoMode(win_w, win_h), bitboard_names[bitboard_vec_index]);
    // } else {
    //     window.setTitle(bitboard_names[bitboard_vec_index]);
    // }
}

void Debug::show_mouse_pos(int x, int y) {
    std::cout << "(" << x << ", " << y << ")" << std::endl;
}