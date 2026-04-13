#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System/String.hpp"

#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH 1280
#define WINDOW_NAME "Chess"
#define BB_WINDOW_NAME "Bitboard Representation"

/* COLORS */

const sf::Color WARM_CREAM(240, 217, 181);
const sf::Color MEDIUM_BROWN(181, 136, 99);
const sf::Color BLACK(0, 0, 0);
const sf::Color WHITE(255, 255, 255);

class Board {

private:

    /* WINDOW */
    const unsigned int win_h;
    const unsigned int win_w;
    const std::string win_name;

    sf::RenderWindow window;
    sf::RenderWindow debug_window;

    
    /* GRID */
    unsigned int board_square_size;
    static constexpr unsigned int GRID_SZ = 8;
    static constexpr unsigned int GRID_NUM_SQUARES = GRID_SZ * GRID_SZ;
    std::vector<sf::RectangleShape> squares;  

    std::string square_names[GRID_NUM_SQUARES] = {
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
    };

    /* BITBOARDS */

    uint64_t w_pawns = 0xFF00ULL;
    uint64_t w_knights = 0x42ULL;
    uint64_t w_bishops = 0x24ULL;
    uint64_t w_rooks = 0x81ULL;
    uint64_t w_queen = 0x10ULL;
    uint64_t w_king = 0x08ULL;

    uint64_t b_pawns = 0x00FF000000000000;
    uint64_t b_knights = 0x4200000000000000;
    uint64_t b_bishops = 0x2400000000000000;
    uint64_t b_rooks = 0x8100000000000000ULL;
    uint64_t b_queen = 0x1000000000000000ULL;
    uint64_t b_king = 0x0800000000000000ULL;

public:

    Board(const unsigned int ww, const unsigned int wh, const std::string wn) : 
    win_w(ww), win_h(wh), win_name(wn), window(sf::VideoMode(win_w, win_h), win_name), 
    bb_window(sf::VideoMode(win_h, win_w), BB_WINDOW_NAME){};

    /* UTIL METHODS PERHAPS IN OTHER FILE TBH */

    void die(std::string err) {
        window.close();
        std::cerr << err << std::endl;
        exit(1);
    }

    sf::Vector2f index_to_2d(int i) {
        return sf::Vector2f(i % GRID_SZ, i / GRID_SZ);
    }
    
    bool is_square_black(int i) {
        sf::Vector2f vec = index_to_2d(i);
        return ((int)vec.x + (int)vec.y) % 2;
    }

    void show_bitboard(uint64_t bitboard) {


        // perhaps this makes a new window.
        for (int i = 0; i < GRID_NUM_SQUARES; i++) {
            int draw_idx = GRID_NUM_SQUARES - i - 1;
            squares[draw_idx].setFillColor(bitboard & (1ULL << i) ? WHITE : BLACK);
        }
    }

    /* INIT */

    void init() {
        
        // Is player black or white?
        // If black, board must be inverted.
        init_get_board_square_size(&board_square_size, win_h, win_w);
        init_board_squares();

        show_bitboard(b_king);
        // coords in the squares perhaps
        // init_board_coords();

        // init_board_start_pieces();

    }

    void init_get_board_square_size(unsigned int *sz, const unsigned win_h, const unsigned win_w) {
        if ((win_h % GRID_SZ) != 0 | (win_w % GRID_SZ) != 0) die("Window size must support eight squares.");
        if (!(win_w == win_h)) die("Window must be square!");

        *sz = win_h / GRID_SZ;
    }

    void init_board_squares() {

        for (int i = 0; i < GRID_NUM_SQUARES; i++) {

            sf::Vector2f pos = index_to_2d(i) * (float)board_square_size;
            squares.emplace_back(sf::Vector2f(board_square_size, board_square_size));

            squares[i].setPosition(pos);
            squares[i].setFillColor(is_square_black(i) ? MEDIUM_BROWN : WARM_CREAM);
        }   
    }
    
    // void init_board_coords() {
    //     // only need to place numbers vert and letters hor.

    //     // if % 8 == 0, then its a number.

    //     for (int i = 0; i < GRID_NUM_SQUARES; i++) {
    //         if ()
    //     }
    // }

    /* RENDER */

    void render() {
        window.clear();

        for (auto& squ : squares) {window.draw(squ);} // render_board_squares();
        // render_board_coords();
        window.display();
    }

    /* RUN */

    void run() {

        sf::Event event;
        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
            }
        }
    };
};