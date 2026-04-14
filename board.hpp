#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Graphics/Image.hpp"

#include "pieces.hpp"
#include "util.hpp"

#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH 1280
#define WINDOW_NAME "Chess"
#define DEBUG_WINDOW_NAME "Debug Window"


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

    bool is_debug;
    sf::RenderWindow debug_window;
    std::vector<sf::RectangleShape> debug_squares;  

    /* BITBOARDS */

    uint64_t w_pawns = 0ULL; // 0xFF00ULL;
    uint64_t w_knights = 0ULL; // 0x42ULL;
    uint64_t w_bishops = 0ULL; //= 0x24ULL;
    uint64_t w_rooks = 0ULL; // 0x81ULL;
    uint64_t w_queen = 0ULL; // 0x10ULL;
    uint64_t w_king = 0ULL; // 0x08ULL;

    uint64_t b_pawns = 0ULL; // 0x00FF000000000000;
    uint64_t b_knights = 0ULL; // 0x4200000000000000;
    uint64_t b_bishops = 0ULL; // 0x2400000000000000;
    uint64_t b_rooks = 0ULL; // 0x8100000000000000ULL;
    uint64_t b_queen = 0ULL; // 0x1000000000000000ULL;
    uint64_t b_king = 0ULL; // 0x0800000000000000ULL;

    #define set_bit(b, i) ((b) |= (1ULL << i))
    #define get_bit(b, i) ((b) & (1ULL << i))
    #define clear_bit(b, i) ((b) &= ~(1ULL << i))
    #define get_LSB(b) (__builtin_ctzll(b))

    /* PIECES */
    
    std::vector<Piece*> pieces;

public:

    Board(const unsigned int ww, const unsigned int wh, const std::string wn, bool db) : 
    win_w(ww), win_h(wh), win_name(wn), window(sf::VideoMode(win_w, win_h), win_name), is_debug(db){};

    /* UTIL METHODS PERHAPS IN OTHER FILE TBH */

    void die(std::string err) {
        window.close();
        std::cerr << err << std::endl;
        exit(1);
    }

    static sf::Vector2f index_to_2d(int i) {
        return sf::Vector2f(i % GRID_SZ, i / GRID_SZ);
    }
    
    bool is_square_black(int i) {
        sf::Vector2f vec = index_to_2d(i);
        return ((int)vec.x + (int)vec.y) % 2;
    }

    /* DEBUG */
    
    void debug() { 
        debug_window.create(sf::VideoMode(win_w, win_h), DEBUG_WINDOW_NAME);
        debug_bitboard(b_queen);
    }

    void debug_bitboard(uint64_t bitboard) {

        // i want this to update in real time eventually.
        for (int i = 0; i < GRID_NUM_SQUARES; i++) {
            int draw_idx = GRID_NUM_SQUARES - i - 1;
            debug_squares[draw_idx].setFillColor(bitboard & (1ULL << i) ? WHITE : BLACK);
        }
    }

    /* INIT */

    void init() {

        // Is player black or white?
        // If black, board must be inverted.
        // grid is drawn, pop up asking black or white, maybe a welcome message. not docked.

        init_get_board_square_size(&board_square_size, win_h, win_w);
        init_board_squares();
        init_pieces();

        if (is_debug) debug();

        // init_board_coords();
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
        // not a fan of the magic numbers for the start positions.
        pieces.push_back(new Bishop(Color::WHITE, window, 58, board_square_size));
        pieces.push_back(new Bishop(Color::WHITE, window, 61, board_square_size));
        set_bit(w_bishops, bitboard_to_draw_index(58));
        set_bit(w_bishops, bitboard_to_draw_index(61));

        pieces.push_back(new Rook(Color::WHITE, window, 63, board_square_size));
        pieces.push_back(new Rook(Color::WHITE, window, 56, board_square_size));
        set_bit(w_rooks, bitboard_to_draw_index(63));
        set_bit(w_rooks, bitboard_to_draw_index(56));

        pieces.push_back(new Knight(Color::WHITE, window, 62, board_square_size));
        pieces.push_back(new Knight(Color::WHITE, window, 57, board_square_size));
        set_bit(w_knights, bitboard_to_draw_index(62));
        set_bit(w_knights, bitboard_to_draw_index(57));

        pieces.push_back(new Queen(Color::WHITE, window, 59, board_square_size));
        set_bit(w_queen, bitboard_to_draw_index(59));

        pieces.push_back(new King(Color::WHITE, window, 60, board_square_size));
        set_bit(w_king, bitboard_to_draw_index(60));


        for (int i = 48; i <= 55; i++) {
            pieces.push_back(new Pawn(Color::WHITE, window, i, board_square_size));
            set_bit(w_pawns, bitboard_to_draw_index(i));
        }  
        
        /* BLACK */

        pieces.push_back(new Bishop(Color::BLACK, window, 2, board_square_size));
        pieces.push_back(new Bishop(Color::BLACK, window, 5, board_square_size));
        set_bit(b_bishops, bitboard_to_draw_index(2));
        set_bit(b_bishops, bitboard_to_draw_index(5));

        pieces.push_back(new Rook(Color::BLACK, window, 0, board_square_size));
        pieces.push_back(new Rook(Color::BLACK, window, 7, board_square_size));
        set_bit(b_rooks, bitboard_to_draw_index(0));
        set_bit(b_rooks, bitboard_to_draw_index(7));

        pieces.push_back(new Knight(Color::BLACK, window, 1, board_square_size));
        pieces.push_back(new Knight(Color::BLACK, window, 6, board_square_size));
        set_bit(b_knights, bitboard_to_draw_index(1));
        set_bit(b_knights, bitboard_to_draw_index(6));

        pieces.push_back(new Queen(Color::BLACK, window, 3, board_square_size));
        set_bit(b_queen, bitboard_to_draw_index(3));

        pieces.push_back(new King(Color::BLACK, window, 4, board_square_size));
        set_bit(b_king, bitboard_to_draw_index(4));


        for (int i = 8; i <= 15; i++) {
            pieces.push_back(new Pawn(Color::BLACK, window, i, board_square_size));
            set_bit(b_pawns, bitboard_to_draw_index(i));
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
        render_main_window();
        if (debug_window.isOpen()) render_debug_window(); 
    }

    void render_main_window() {

        window.clear();
        for (auto& squ : squares) {window.draw(squ);}


        for (auto& piece : pieces) piece->draw(window);
        
        // go through associated bitboard and fill out squares with piece.
        // render_board_coords();
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
        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
            }

            if (debug_window.isOpen()) {
                while (debug_window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) debug_window.close();
                }
            }
            render();     
        }
    }
};