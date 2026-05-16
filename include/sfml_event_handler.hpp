#pragma once

#include "SFML/Graphics.hpp"
#include "board.hpp"

#include <functional>

class SFMLEventHandler {
private:

    Board& board;
    sf::RenderWindow& main_window;
    uint16_t board_square_size;

    std::function<void(uint8_t)> reset_highlights;


public:
    SFMLEventHandler(Board& board,
                     sf::RenderWindow& main_win, 
                     uint16_t squ_sz, 
                     std::function<void(uint8_t)> reset_highlights);

    int mouse_win_pos_to_bit();

    void handle_events();
    void on_main_window_event(sf::Event &event);
    void on_key_pressed(sf::Event &event);
    void on_mouse_press(sf::Event &event);
    void on_left_mouse_press();
};