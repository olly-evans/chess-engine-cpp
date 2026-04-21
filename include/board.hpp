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
    

    // All bitboards.
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

    bool is_piece_highlighted = false;
    Piece* highlighted_piece = nullptr;

public:

    Board(const unsigned int ww, const unsigned int wh, const std::string wn);

    unsigned int board_square_size;

    /* UTIL METHODS */

    void die(const std::string& err);
    bool is_square_black(int i);
    int mouse_win_pos_to_square_index();
    void reset_square_color(sf::Vector2f square);


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
};