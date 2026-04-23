#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Graphics/Image.hpp"

#include "pieces.hpp"
#include "debug.hpp"

#include <sstream>
#include <iostream>
#include <cmath>

constexpr auto WINDOW_HEIGHT = 1280;
constexpr auto WINDOW_WIDTH = 1280;
constexpr auto WINDOW_NAME = "Chess";
static_assert(WINDOW_HEIGHT == WINDOW_WIDTH, "Window must be square.");

#define NAME_OF(x) #x

/* COLORS */

const sf::Color WARM_CREAM(240, 217, 181);
const sf::Color MEDIUM_BROWN(181, 136, 99);
const sf::Color BLACK(0, 0, 0);
const sf::Color WHITE(255, 255, 255);
const sf::Color TURQOISE(50, 200, 150);

// class AbstractBoard {
//     virtual void die(std::string) = 0;
//     virtual void index_to_2d(int i) = 0;
//     .
//     .
//     .
// };

class Board {

private:

    /* WINDOW */
    
    const std::string win_name;
    const unsigned int win_h;
    const unsigned int win_w;

    sf::RenderWindow main_window;
    
    /* GRID */
    std::vector<sf::RectangleShape> squares;

    // White player.
    enum WPSquares : uint8_t {
        A8 = 63, B8 = 62, C8 = 61, D8 = 60, E8 = 59, F8 = 58, G8 = 57, H8 = 56,
        A7 = 55, B7 = 54, C7 = 53, D7 = 52, E7 = 51, F7 = 50, G7 = 49, H7 = 48,
        A6 = 47, B6 = 46, C6 = 47, D6 = 44, E6 = 43, F6 = 42, G6 = 41, H6 = 40,
        A5 = 39, B5 = 38, C5 = 37, D5 = 36, E5 = 35, F5 = 34, G5 = 33, H5 = 32,
        A4 = 31, B4 = 30, C4 = 29, D4 = 28, E4 = 27, F4 = 26, G4 = 25, H4 = 24,
        A3 = 23, B3 = 22, C3 = 21, D3 = 20, E3 = 19, F3 = 18, G3 = 17, H3 = 16,
        A2 = 15, B2 = 14, C2 = 13, D2 = 12, E2 = 11, F2 = 10, G2 = 9, H2 = 8,
        A1 = 7, B1 = 6, C1 = 5, D1 = 4, E1 = 3, F1 = 2, G1 = 1, H1 = 0 
    };

    /* DEBUG */

    sf::RenderWindow bitboard_window;
    std::vector<sf::RectangleShape> bitboard_window_squares;

    /* BITBOARDS */

    /*

    uint64_t w_pawns bitboard initial positions example.

        a    b    c    d    e    f    g    h
    +----+----+----+----+----+----+----+----+
    | MSB|    |    |    |    |    |    |    | 8
    +----+----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    |    | 7
    +----+----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    |    | 6
    +----+----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    |    | 5
    +----+----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    |    | 4
    +----+----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    |    | 3
    +----+----+----+----+----+----+----+----+
    |  1 |  1 |  1 |  1 |  1 |  1 |  1 |  1 | 2
    +----+----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    | LSB| 1
    +----+----+----+----+----+----+----+----+

    */

    // Vector of all bitboards.
    std::vector<uint64_t> bitboards;
    std::vector<std::string> bitboard_names;
    int bitboard_vec_index = 0;
    

    // Starting piece bitboards.
    uint64_t w_pawns = 0xFF00ULL;
    uint64_t w_knights = 0x42ULL;
    uint64_t w_bishops = 0x24ULL;
    uint64_t w_rooks = 0x81ULL;
    uint64_t w_queen = 0x10ULL;
    uint64_t w_king = 0x08ULL;

    uint64_t b_pawns = 0x00FF000000000000ULL;
    uint64_t b_knights = 0x4200000000000000ULL;
    uint64_t b_bishops = 0x2400000000000000ULL;
    uint64_t b_rooks = 0x8100000000000000ULL;
    uint64_t b_queen = 0x1000000000000000ULL;
    uint64_t b_king = 0x0800000000000000ULL;

    /* PIECES */
    
    // Only two instances of each piece for black or white, black and white Pawn instance for example.
    std::vector<Piece*> pieces;

    Piece* selected_piece = nullptr;

public:

    Board(const unsigned int ww, const unsigned int wh, const std::string wn);

    unsigned int board_square_size;

    /* UTIL METHODS */

    void die(const std::string& err);
    bool is_square_black(int i);
    int mouse_win_pos_to_bit();
    void reset_square_color(int square);

    bool move_square_has_friendly_piece(Color col, sf::Vector2f move);

    uint64_t white_occupancy();
    uint64_t black_occupancy();


    int square_index_to_piece_type_index(int square_index);

    // only here in case we pass board to pieces, well maybe other reasons too eventually.
    const unsigned int get_win_width();

    /* INIT */

    void init();
    void init_bitboards();
    void init_get_board_square_size(uint32_t& sz, const unsigned win_h, const unsigned win_w);
    void init_main_window_squares();
    void init_bitboard_window_squares();
    void init_pieces();

    /* RENDER */

    void render();
    void render_main_window();
    void render_bitboard_window();

    /* RUN */

    void run();

    /* RUN -> EVENT HANDLING */

    void run_handle_events();
    void on_main_window_event(sf::Event &event);
    void on_bitboard_window_event(sf::Event &event);
    void on_key_pressed(sf::Event &event);
    void on_mouse_press(sf::Event &event);
    void on_left_mouse_press();
    void deselect_piece();
    Piece* select_piece(uint8_t bit);
    Piece* get_piece(uint8_t clicked_bit);

};