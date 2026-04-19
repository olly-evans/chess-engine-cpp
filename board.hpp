#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Graphics/Image.hpp"

#include "pieces.hpp"
#include "util.hpp"
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
    enum PieceType { W_PAWNS, W_KNIGHTS, W_BISHOPS, W_ROOKS, W_QUEEN, W_KING,
                 B_PAWNS, B_KNIGHTS, B_BISHOPS, B_ROOKS, B_QUEEN, B_KING };

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
    std::vector<Piece*> piece_types;

public:

    Board(const unsigned int ww, const unsigned int wh, const std::string wn) : 
    win_w(ww), 
    win_h(wh), 
    win_name(std::move(wn)), 
    main_window(sf::VideoMode(win_w, win_h), 
    win_name) {};

    unsigned int board_square_size;

    /* UTIL METHODS */

    void die(const std::string& err) {
        main_window.close();
        std::cerr << err << std::endl;
        exit(1);
    }
    
    bool is_square_black(int i) {
        sf::Vector2f vec = index_to_2d(i);
        return ((int)vec.x + (int)vec.y) % 2;
    }

    int mouse_win_pos_to_piece_vec_index() {
        // May need to be broken up into smaller functions.
        sf::Vector2i mouse_window_pos = sf::Mouse::getPosition(main_window);
        sf::Vector2i mouse_square_pos((mouse_window_pos.x / board_square_size), 
                                      (mouse_window_pos.y / board_square_size));
        return (mouse_square_pos.y * GRID_SZ) + mouse_square_pos.x;
    }

    bool is_bit_set(uint64_t bitboard, int index) {
        return (bitboard & (1ULL << index)) != 0;
    }

    int piece_at(int square_index) {

        // this function might as well return piece pointer.
        /* return: int index for piece in piece_type vector. */
        int bitboard_idx = GRID_NUM_SQUARES - square_index - 1;

        for (int i = 0; i < piece_types.size(); i++) {
            // condition might be dodge.

            // input is zero here, which would be 0,0.
            if (is_bit_set(piece_types.at(i)->bitboard, bitboard_idx)) return i;
        }

        return -1;
    }

    //     for (int i = 0; i < bitboards.size(); i++) {
    //             if (get_bit(bitboards[i], index) == 1) return            
    //         }
    //     }
    // }

    // for (auto& piece : piece_types) {
    //     for (auto& position : piece.piece_positions) {

    //     }

    // }


    

    // only here in case we pass board to pieces, well maybe other reasons too eventually.
    const unsigned int get_win_width() {
        return win_w; 
    }

    /* INIT */

    void init() {

        // Is player black or white?
        // If black, board must be inverted.
        // grid is drawn, pop up asking black or white, maybe a welcome message. not docked.
        init_bitboards();
        init_get_board_square_size(board_square_size, win_h, win_w);
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

    void init_get_board_square_size(uint32_t& sz, const unsigned win_h, const unsigned win_w) {
        if ((win_h % GRID_SZ) != 0 | (win_w % GRID_SZ) != 0) die("Window size must support eight squares.");
        sz = win_h / GRID_SZ;
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

        piece_types.emplace_back(new Pawn(Color::WHITE, main_window, w_pawns, board_square_size));
        piece_types.emplace_back(new Knight(Color::WHITE, main_window, w_knights, board_square_size));
        piece_types.emplace_back(new Bishop(Color::WHITE, main_window, w_bishops, board_square_size));
        piece_types.emplace_back(new Rook(Color::WHITE, main_window, w_rooks, board_square_size));
        piece_types.emplace_back(new Queen(Color::WHITE, main_window, w_queen, board_square_size));
        piece_types.emplace_back(new King(Color::WHITE, main_window, w_king, board_square_size));

        /* BLACK */

        piece_types.emplace_back(new Pawn(Color::BLACK, main_window, b_pawns, board_square_size));
        piece_types.emplace_back(new Knight(Color::BLACK, main_window, b_knights, board_square_size));
        piece_types.emplace_back(new Bishop(Color::BLACK, main_window, b_bishops, board_square_size));
        piece_types.emplace_back(new Rook(Color::BLACK, main_window, b_rooks, board_square_size));
        piece_types.emplace_back(new Queen(Color::BLACK, main_window, b_queen, board_square_size));
        piece_types.emplace_back(new King(Color::BLACK, main_window, b_king, board_square_size));
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
        
        // std::cout << "render_main_window()" << std::endl;

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
        // changed this to if from while and it didn't seem to do anything.
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
        if (event.type == sf::Event::MouseButtonPressed) on_mouse_press(event);   
    }

    void on_bitboard_window_event(sf::Event &event) {
        if (event.type == sf::Event::Closed) bitboard_window.close();   
    }

    void on_key_pressed(sf::Event &event) {

        auto key = event.key.code;

        if (key == sf::Keyboard::Tab) {
            if (!bitboard_window.isOpen() && Debug::enabled)
                // Cycle through bitboard vector and change title of bitboard_window.
                bitboard_window.create(sf::VideoMode(win_w, win_h), bitboard_names[bitboard_vec_index]);
                bitboard_vec_index = (bitboard_vec_index + 1) % bitboards.size();
                bitboard_window.setTitle(bitboard_names[bitboard_vec_index]);            
        }
    }

    void on_mouse_press(sf::Event &event) {

        auto mouse_press = event.mouseButton.button;

        if (mouse_press == sf::Mouse::Left) {

            int square_index = mouse_win_pos_to_piece_vec_index();
            std::cout << square_index << "\n";
            // find the piece thats there.
            // something bitwise with this index. ^
            uint64_t piece_index = piece_at(square_index);

            if (piece_index == -1) return;
            
            sf::Vector2f pos = index_to_2d(square_index);

            std::cout << "piece_index: " << piece_index << "\n";
            piece_types[piece_index]->highlight_piece(pos);

            // piece_selected();
                // .highlight();
                // .show_moves();
        }
    }
};