#include "debug.hpp"
#include "board.hpp"
#include <iostream>

// Pass in the bitboard AND the array of squares it needs to color
void Debug::draw_bitboard(uint64_t bitboard, sf::RectangleShape* squares) {
    
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        int draw_idx = GRID_NUM_SQUARES - i - 1;
        squares[draw_idx].setFillColor(bitboard & (1ULL << i) ? sf::Color::White : sf::Color::Black);
    }
}

// Pass in the raw coordinates
void Debug::mouse_pos(int x, int y) {
    // We don't even need stringstream here anymore if we just want to print!
    std::cout << "(" << x << ", " << y << ")" << std::endl;
}