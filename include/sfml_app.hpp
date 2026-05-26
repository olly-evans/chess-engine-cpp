#pragma once

#include "SFML/Graphics.hpp"

#include "piece.hpp"
#include "board.hpp"

#include <iostream>
#include <cstdint>
#include <unordered_map>

constexpr uint8_t GRID_SZ = 8;
constexpr uint8_t GRID_NUM_SQUARES = GRID_SZ * GRID_SZ;

constexpr uint16_t WINDOW_WIDTH = 1280;

constexpr auto WINDOW_NAME = "Chess";

const sf::Color WARM_CREAM(229, 195, 165);
const sf::Color MEDIUM_BROWN(201, 126, 66);
const sf::Color BLACK(0, 0, 0);
const sf::Color WHITE(255, 255, 255);
const sf::Color TURQUOISE(50, 200, 150);
const sf::Color SKY_BLUE(61, 153, 212);

class SFMLApp {
private:

    const uint16_t win_w;
    uint16_t board_square_size;    
    
    sf::RenderWindow main_window;
    
    Board& board;

    std::vector<sf::RectangleShape> squares;

    std::unordered_map<char, sf::Texture> texture_cache;  

public:

    SFMLApp(Board& board, const uint16_t w_width);

    bool is_square_black(uint8_t bit);

    void init();
    
    void set_board_square_size(uint16_t& sz);
    void init_main_window_squares();
    void reset_main_window_squares();

    std::string resolve_texture_path(char id);
    void load_textures();
    void init_piece_texture_cache();


    
    uint16_t get_board_square_size();
    sf::RenderWindow& get_main_window();

    void run(); // Main game loop.

    /* RENDERING*/

    void render(); 
    void render_main_window();
    void render_pieces();
 
    void render_move_highlights();
    void render_capture_highlights();

    void reset_move_and_capture_highlights(uint8_t selected_bit);

    /* EVENT HANDLING */

    uint32_t mouse_win_pos_to_bit();

    void handle_events();
    void on_main_window_event(sf::Event &event);
    void on_key_pressed(sf::Event &event);
    void on_mouse_press(sf::Event &event);
    void on_left_mouse_press();
};