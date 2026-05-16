#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Graphics/Image.hpp"

#include "pieces.hpp"
#include "debug.hpp"
#include "player.hpp"
#include "fenparser.hpp"

#include <sstream>
#include <iostream>
#include <cmath>

constexpr auto WINDOW_HEIGHT = 1280;
constexpr auto WINDOW_WIDTH = 1280;
static_assert(WINDOW_HEIGHT == WINDOW_WIDTH, "Window must be square.");

constexpr auto WINDOW_NAME = "Chess";

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

    /* GAMESTATE */
    
    // Player* white_player;
    // Player* black_player;

    bool is_whites_turn = true;

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

    int bitboard_vec_index = 0;
    
    uint64_t w_pawns = 0ULL;
    uint64_t w_knights = 0ULL;
    uint64_t w_bishops = 0ULL;
    uint64_t w_rooks = 0ULL;
    uint64_t w_queen = 0ULL;
    uint64_t w_king = 0ULL;

    uint64_t b_pawns = 0ULL;
    uint64_t b_knights = 0ULL;
    uint64_t b_bishops = 0ULL;
    uint64_t b_rooks = 0ULL;
    uint64_t b_queen = 0ULL;
    uint64_t b_king = 0ULL;

    /* PIECES */
    
    std::vector<Piece*> pieces;
    Piece* selected_piece = nullptr;

public:

    Board(const unsigned int ww, const unsigned int wh, const std::string wn);

    unsigned int board_square_size;

    // Vector of all bitboards.
    static std::vector<uint64_t> bitboards; // White in order of piece value, then black in order of piece value. 12 elements.
    std::vector<char> bitboard_names;

    /* UTIL METHODS */

    void die(const std::string& err);
    bool is_square_black(uint8_t i);
    int mouse_win_pos_to_bit();

    uint64_t white_occupancy();
    uint64_t black_occupancy();

    uint64_t get_white_captures(uint64_t white, uint64_t black); // omve if works, tmp position.
    uint64_t get_black_captures(uint64_t white, uint64_t black); // also tmp, just wanna see shit.

    uint64_t get_simulated_enemy_captures(Piece* piece, uint8_t start, uint8_t end, uint8_t capture);

    bool white_king_in_check(uint64_t white, uint64_t black);
    bool black_king_in_check(uint64_t white, uint64_t black);

    void king_in_check(uint64_t white, uint64_t black);

    void update_all_piece_attacks();



    /* INIT */

    void init();
    void init_players();
    void init_bitboards_from_fen(std::string fen);

    void init_get_board_square_size(uint32_t& sz, const unsigned win_h, const unsigned win_w);
    
    void init_main_window_squares();

    void init_bitboard_window_squares();
    void init_position_from_fen(std::string fen);

    void create_piece(const char ch, uint8_t bit);

    void init_pieces();

    /* RENDER */

    void render();
    void render_main_window();
    void render_move_highlights();
    void render_capture_highlights();

    void render_bitboard_window();

    /* RUN */

    void run();

    /* EVENT HANDLING */

    void handle_events();
    void on_main_window_event(sf::Event &event);
    void on_bitboard_window_event(sf::Event &event);

    void on_key_pressed(sf::Event &event);
    void undo_move();

    void on_mouse_press(sf::Event &event);
    void on_left_mouse_press();
    void reset_move_and_capture_highlights(uint8_t selected_bit);

    /* PIECE FUNCTIONALITY */

    Piece* select_piece(uint8_t bit);
    Piece* get_piece(uint8_t clicked_bit);
    bool bit_has_piece(uint8_t clicked_bit);
    void remove_piece(uint8_t piece_to_remove_bit);

    void handle_piece_move(uint8_t clicked_bit);

    bool is_enpassant_capture(uint8_t clicked_bit);

    /* FREES */

    void free_pieces();

};