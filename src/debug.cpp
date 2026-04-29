#include "debug.hpp"
#include "board.hpp"
#include <iostream>

void Debug::draw_cycle_bitboard(sf::RenderWindow &window, int win_w, int win_h, std::vector<uint64_t> bitboards, std::vector<std::string> bitboard_names, int bitboard_vec_index, std::vector<sf::RectangleShape> &squares) {
    // assert(bitboard.size() == bitboard_names.size())
    
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        int draw_idx = GRID_NUM_SQUARES - i - 1;
        squares[draw_idx].setFillColor(bitboards[bitboard_vec_index] & (1ULL << i) ? sf::Color::White : sf::Color::Black);
    }
}

// void Debug::draw_bitboard(uint64_t bb,
//                           sf::RenderWindow& window, 
//                           int win_w, 
//                           int win_h, 
//                           std::vector<sf::RectangleShape>& squares) 
// {
//     for (int i = 0; i < GRID_NUM_SQUARES; i++) {
//         int draw_idx = GRID_NUM_SQUARES - i - 1;
//         squares[draw_idx].setFillColor(bb & (1ULL << i) ? WHITE : BLACK);
//     }

// }

void Debug::draw_bitboard(sf::RenderWindow& window, uint64_t bitboard, std::vector<sf::RectangleShape>& squares) {
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        int draw_idx = GRID_NUM_SQUARES - i - 1;
        squares[draw_idx].setFillColor(bitboard & (1ULL << i) ? WHITE : BLACK);
    }
}