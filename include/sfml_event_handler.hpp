#pragma once

#include "SFML/Graphics.hpp"


class SFMLEventHandler {
private:

    sf::RenderWindow& main_window;

public:
    SFMLEventHandler(sf::RenderWindow& main_win);

    void handle_events();
    void on_main_window_event(sf::Event &event);
    void on_key_pressed(sf::Event &event);
    void on_mouse_press(sf::Event &event);
    // void on_left_mouse_press();
    void reset_move_and_capture_highlights(uint8_t selected_bit);
};