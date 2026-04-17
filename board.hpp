#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Graphics/Image.hpp"

#include "pieces.hpp"
#include "util.hpp"
#include "debug.hpp"

#include <sstream>
#include <iostream>

// Have 80px piece pngs, smallest recommended size would be 640px.
#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH 1280
#define WINDOW_NAME "Chess"

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
    
    const std::string win_name;
    const unsigned int win_h;
    const unsigned int win_w;

    sf::RenderWindow main_window;

    /* MOUSE */

    int mouse_x;
    int mouse_y;
    
    /* GRID */
    std::vector<sf::RectangleShape> squares;

    /* DEBUG */

    bool debug_enabled;
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

    // Bitboard macros.
    #define set_bit(b, i) ((b) |= (1ULL << i))
    #define get_bit(b, i) ((b) & (1ULL << i))
    #define clear_bit(b, i) ((b) &= ~(1ULL << i))
    #define get_LSB(b) (__builtin_ctzll(b))

    /* PIECES */
    
    // Only two instances of each piece for black or white, black and white Pawn instance for example.
    std::vector<Piece*> piece_types;

public:

    Board(const unsigned int ww, const unsigned int wh, const std::string wn, bool db) : 
    win_w(ww), 
    win_h(wh), 
    win_name(wn), 
    main_window(sf::VideoMode(win_w, win_h), 
    win_name), 
    debug_enabled(db){};

    unsigned int board_square_size;

    
    /* UTIL METHODS */

    void die(std::string err) {
        main_window.close();
        std::cerr << err << std::endl;
        exit(1);
    }
    
    bool is_square_black(int i) {
        sf::Vector2f vec = index_to_2d(i);
        return ((int)vec.x + (int)vec.y) % 2;
    }

    // only here in case we pass board to pieces.
    const unsigned int get_win_w_h() {
        return win_h | win_w; // Equal.
    }

    /* INIT */

    void init() {

        // Is player black or white?
        // If black, board must be inverted.
        // grid is drawn, pop up asking black or white, maybe a welcome message. not docked.
        init_bitboards();
        init_get_board_square_size(&board_square_size, win_h, win_w);
        init_main_window_squares();

        // I like this for now. Keeps it in init and only runs if debug enabled.
        if (Debug::enabled)
            init_bitboard_window_squares();

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

    void init_main_window_squares() {

        for (int i = 0; i < GRID_NUM_SQUARES; i++) {
            sf::Vector2f pos = index_to_2d(i) * (float)board_square_size;
            squares.emplace_back(sf::Vector2f(board_square_size, board_square_size));
            squares[i].setPosition(pos);
            squares[i].setFillColor(is_square_black(i) ? MEDIUM_BROWN : WARM_CREAM);
        }   
    }

    void init_bitboard_window_squares() {

        for (int i = 0; i < GRID_NUM_SQUARES; i++) {
            sf::Vector2f pos = index_to_2d(i) * (float)board_square_size;
            bitboard_window_squares.emplace_back(sf::Vector2f(board_square_size, board_square_size));
            bitboard_window_squares[i].setPosition(pos);
        }   
    }

    void init_pieces() {

        // if player = black_pieces
            // different starting pos.
            // prob a bitwise operation for that.

        // else        

        /* WHITE */

        piece_types.push_back(new King(Color::WHITE, main_window, w_king, board_square_size));
        piece_types.push_back(new Queen(Color::WHITE, main_window, w_queen, board_square_size));
        piece_types.push_back(new Rook(Color::WHITE, main_window, w_rooks, board_square_size));
        piece_types.push_back(new Bishop(Color::WHITE, main_window, w_bishops, board_square_size));
        piece_types.push_back(new Knight(Color::WHITE, main_window, w_knights, board_square_size));
        piece_types.push_back(new Pawn(Color::WHITE, main_window, w_pawns, board_square_size));

        /* BLACK */

        piece_types.push_back(new King(Color::BLACK, main_window, b_king, board_square_size));
        piece_types.push_back(new Queen(Color::BLACK, main_window, b_queen, board_square_size));
        piece_types.push_back(new Rook(Color::BLACK, main_window, b_rooks, board_square_size));
        piece_types.push_back(new Bishop(Color::BLACK, main_window, b_bishops, board_square_size));
        piece_types.push_back(new Knight(Color::BLACK, main_window, b_knights, board_square_size));
        piece_types.push_back(new Pawn(Color::BLACK, main_window, b_pawns, board_square_size));
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
        if (Debug::enabled) render_bitboard_window(); 
    }

    void render_main_window() {
        main_window.clear();
        for (auto& squ : squares) {main_window.draw(squ);}

        // render_board_coords();

        for (auto& piece_type : piece_types) {piece_type->draw(main_window);}
        main_window.display();
    }

    void render_bitboard_window() {
        bitboard_window.clear();
        for (auto& squ : bitboard_window_squares) {bitboard_window.draw(squ);}
        Debug::draw_cycle_bitboard(bitboard_window, win_w, win_h, bitboards, bitboard_names, bitboard_vec_index, bitboard_window_squares);
        bitboard_window.display();
    }

    /* RUN */

    void run() {
        while (main_window.isOpen()) {
            run_handle_events();
            render();     
        }
    }

    /* RUN -> EVENT HANDLING */

    void run_handle_events() {
        sf::Event event;
        while (main_window.pollEvent(event)) {
            on_main_window_event(event);
        }

        if (bitboard_window.isOpen()) {
            sf::Event debug_event;
            while (bitboard_window.pollEvent(debug_event)) {
                on_bitboard_window_event(debug_event);
            }
        }
    }

    void on_main_window_event(sf::Event &event) {
        if (event.type == sf::Event::Closed) main_window.close();
        if (event.type == sf::Event::KeyPressed) on_key_pressed(event);
    }

    void on_bitboard_window_event(sf::Event &event) {
        if (event.type == sf::Event::Closed) bitboard_window.close();   
    }

    void on_key_pressed(sf::Event &event) {

        auto key = event.key.code;
        
        if (key == sf::Keyboard::Tab) {
            if (!bitboard_window.isOpen() && Debug::enabled)
                bitboard_window.create(sf::VideoMode(win_w, win_h), bitboard_names[bitboard_vec_index]);
                bitboard_vec_index = (bitboard_vec_index + 1) % bitboards.size();
                bitboard_window.setTitle(bitboard_names[bitboard_vec_index]);            
        }
    }
};