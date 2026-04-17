#include "debug.hpp"
#include "board.hpp"
#include <iostream>
// #include <assert>

void Debug::draw_cycle_bitboard(sf::RenderWindow &window, int win_w, int win_h, std::vector<uint64_t> bitboards, std::vector<std::string> bitboard_names, int bitboard_vec_index, std::vector<sf::RectangleShape> &squares) {
    // assert(bitboard.size() == bitboard_names.size())
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        int draw_idx = GRID_NUM_SQUARES - i - 1;
        squares[draw_idx].setFillColor(bitboards[bitboard_vec_index] & (1ULL << i) ? sf::Color::White : sf::Color::Black);
    }
}

void Debug::show_mouse_pos(int x, int y) {
    std::cout << "(" << x << ", " << y << ")" << std::endl;
}