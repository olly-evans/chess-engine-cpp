#pragma once

#include "SFML/Graphics.hpp"
#include "sfml_event_handler.hpp"

#include "pieces.hpp"
#include "board.hpp"

#include <iostream>
#include <cstdint>

constexpr uint8_t GRID_SZ = 8;
constexpr uint8_t GRID_NUM_SQUARES = GRID_SZ * GRID_SZ;

constexpr uint16_t WINDOW_HEIGHT = 1280;
constexpr uint16_t WINDOW_WIDTH = 1280;
static_assert(WINDOW_HEIGHT == WINDOW_WIDTH, "Window must be square.");

constexpr auto WINDOW_NAME = "Chess";

const sf::Color WARM_CREAM(240, 217, 181);
const sf::Color MEDIUM_BROWN(181, 136, 99);
const sf::Color BLACK(0, 0, 0);
const sf::Color WHITE(255, 255, 255);
const sf::Color TURQOISE(50, 200, 150);

class SFMLRenderer {
private:

    const uint16_t win_w;
    
    
    sf::RenderWindow main_window;

    SFMLEventHandler event_handler;
    Board board;

    uint16_t board_square_size;

    std::vector<sf::RectangleShape> squares;

    Piece* selected_piece = nullptr;


public:

    SFMLRenderer(const uint16_t w_width);

    void init_renderer();
    
    bool is_square_black(uint8_t bit);
    
    void set_board_square_size(uint16_t& sz);
    uint16_t get_board_square_size();

    void set_main_window_squares();

    sf::RenderWindow& get_main_window();


    void run(); // Main game loop.

    void render(); 
    void render_main_window();
    void render_move_highlights();
    void render_capture_highlights();

};