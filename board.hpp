#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Graphics/Image.hpp"

#include "pieces.hpp"
#include "util.hpp"
#include "debug.hpp"

#include <sstream>
#include <iostream>

#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH 1280
#define WINDOW_NAME "Chess"
#define DEBUG_WINDOW_NAME "Debug Window"

#define NAME_OF(x) #x


/* COLORS */

const sf::Color WARM_CREAM(240, 217, 181);
const sf::Color MEDIUM_BROWN(181, 136, 99);
const sf::Color BLACK(0, 0, 0);
const sf::Color WHITE(255, 255, 255);

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
    const unsigned int win_h;
    const unsigned int win_w;
    const std::string win_name;

    sf::RenderWindow window;

    /* MOUSE */

    int mouse_x;
    int mouse_y;
    
    /* GRID */
    unsigned int board_square_size;
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

    /* DEBUG */

    bool debug_enabled;
    sf::RenderWindow debug_window;
    std::vector<sf::RectangleShape> debug_squares;

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

    std::vector<uint64_t> bitboards;
    std::vector<std::string> bitboard_names;

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

    #define set_bit(b, i) ((b) |= (1ULL << i))
    #define get_bit(b, i) ((b) & (1ULL << i))
    #define clear_bit(b, i) ((b) &= ~(1ULL << i))
    #define get_LSB(b) (__builtin_ctzll(b))

    /* PIECES */
    
    // Only two instances of each piece for black or white.
    std::vector<Piece*> active_pieces;

public:

    Board(const unsigned int ww, const unsigned int wh, const std::string wn, bool db) : 
    win_w(ww), win_h(wh), win_name(wn), window(sf::VideoMode(win_w, win_h), win_name), debug_enabled(db){};

    /* UTIL METHODS PERHAPS IN OTHER FILE TBH */

    void die(std::string err) {
        window.close();
        std::cerr << err << std::endl;
        exit(1);
    }
    
    bool is_square_black(int i) {
        sf::Vector2f vec = index_to_2d(i);
        return ((int)vec.x + (int)vec.y) % 2;
    }

    /* INIT */

    void init() {

        // Is player black or white?
        // If black, board must be inverted.
        // grid is drawn, pop up asking black or white, maybe a welcome message. not docked.
        init_bitboards();
        init_get_board_square_size(&board_square_size, win_h, win_w);
        init_board_squares();
        init_pieces();
        // init_board_coords();
    }

    void init_bitboards() {
        bitboards = {
            w_pawns, w_knights, w_bishops, w_rooks, w_queen, w_king,
            b_pawns, b_knights, b_bishops, b_rooks, b_queen, b_king
        };

        bitboard_names = {
            NAME_OF(w_pawns), NAME_OF(w_knights), NAME_OF(w_bishops), NAME_OF(w_rooks), NAME_OF(w_queen), NAME_OF(w_king),
            NAME_OF(b_pawns), NAME_OF(b_knights), NAME_OF(b_bishops), NAME_OF(b_rooks), NAME_OF(b_queen), NAME_OF(b_king)
        };

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

            debug_squares.emplace_back(sf::Vector2f(board_square_size, board_square_size));
            debug_squares[i].setPosition(pos);

            squares[i].setFillColor(is_square_black(i) ? MEDIUM_BROWN : WARM_CREAM);
        }   
    }

    void init_pieces() {

        // if player = black_pieces
            // different starting pos.
            // prob a bitwise operation for that.

        // else        

        /* WHITE */

        active_pieces.push_back(new King(Color::WHITE, window, w_king, board_square_size));
        active_pieces.push_back(new Queen(Color::WHITE, window, w_queen, board_square_size));
        active_pieces.push_back(new Rook(Color::WHITE, window, w_rooks, board_square_size));
        active_pieces.push_back(new Bishop(Color::WHITE, window, w_bishops, board_square_size));
        active_pieces.push_back(new Knight(Color::WHITE, window, w_knights, board_square_size));
        active_pieces.push_back(new Pawn(Color::WHITE, window, w_pawns, board_square_size));

        /* BLACK */

        active_pieces.push_back(new King(Color::BLACK, window, b_king, board_square_size));
        active_pieces.push_back(new Queen(Color::BLACK, window, b_queen, board_square_size));
        active_pieces.push_back(new Rook(Color::BLACK, window, b_rooks, board_square_size));
        active_pieces.push_back(new Bishop(Color::BLACK, window, b_bishops, board_square_size));
        active_pieces.push_back(new Knight(Color::BLACK, window, b_knights, board_square_size));
        active_pieces.push_back(new Pawn(Color::BLACK, window, b_pawns, board_square_size));
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
        render_main_window();
        render_debug_window(); 
    }

    void render_main_window() {

        window.clear();
        for (auto& squ : squares) {window.draw(squ);}
        // render_board_coords();

        for (auto& piece_type : active_pieces) piece_type->draw(window);
        window.display();
    }

    void render_debug_window() {
        debug_window.clear();
        for (auto& squ : debug_squares) {debug_window.draw(squ);}
        debug_window.display();
    }

    /* RUN */

    void run() {
    sf::Event event;
    int current_idx = 0; 

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) 
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab) {
                    current_idx = (current_idx + 1) % bitboards.size();

                    if (!debug_window.isOpen()) {
                        debug_window.create(sf::VideoMode(win_w, win_h), bitboard_names[current_idx]);
                    } else {
                        debug_window.setTitle(bitboard_names[current_idx]);
                    }
                }
                
                if (event.key.code == sf::Keyboard::Escape) {
                    debug_window.close();
                }
            }
        }

        mouse_x = sf::Mouse::getPosition(window).x;
        mouse_y = sf::Mouse::getPosition(window).y;

        if (debug_window.isOpen()) {
            Debug::draw_bitboard(bitboards[current_idx], debug_squares);
            
            // Standard pollEvent for the second window to keep it responsive
            sf::Event debugEvent;
            while (debug_window.pollEvent(debugEvent)) {
                if (debugEvent.type == sf::Event::Closed) {
                    debug_window.close();
                }
            }
        }

        render();     
    }
}
};