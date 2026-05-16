#pragma once

#include "SFML/Graphics.hpp"

#include <iostream>
#include <cstdint>

// constexpr unsigned uint8_t GRID_SZ = 8;
// constexpr unsigned uint8_t GRID_NUM_SQUARES = GRID_SZ * GRID_SZ;

class SFMLRenderer {
private:

    sf::RenderWindow main_window;
    std::vector<sf::RectangleShape> squares;

    // sf::RenderWindow bitboard_window;
    // std::vector<sf::RectangleShape> bitboard_window_squares;
    // uint8_t bitboard_vec_index = 0;

public:
    void init_renderer();
    
    bool is_square_black(uint8_t bit);
    
    void set_board_square_size(uint32_t& sz, const unsigned win_h, const unsigned win_w);
    void set_main_window_squares();

    sf::RenderWindow& get_main_window();

};