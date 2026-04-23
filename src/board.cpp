#include "SFML/Graphics.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Graphics/Image.hpp"

#include "pieces.hpp"
#include "debug.hpp"
#include "bitboardhelper.hpp"

#include "board.hpp"

#include <sstream>
#include <iostream>
#include <cmath>



// class AbstractBoard {
//     virtual void die(std::string) = 0;
//     virtual void index_to_2d(int i) = 0;
//     .
//     .
//     .
// };


Board::Board(const unsigned int ww, const unsigned int wh, const std::string wn) : 
win_w(ww), 
win_h(wh), 
win_name(std::move(wn)), 
main_window(sf::VideoMode(win_w, win_h), 
win_name) {

    // who is white who is black..

    // SquareToBits enum updated depending on board orientation.
};

unsigned int board_square_size;

/* UTIL METHODS */

void Board::die(const std::string& err) {
    main_window.close();
    std::cerr << err << std::endl;
    exit(1);
}

bool Board::is_square_black(int i) {
    sf::Vector2f vec = index_to_2d(i);
    return ((int)vec.x + (int)vec.y) % 2;
}

int Board::mouse_win_pos_to_bit() {
    // May need to be broken up into smaller functions.
    sf::Vector2i mouse_window_pos = sf::Mouse::getPosition(main_window);
    sf::Vector2i mouse_square_pos((mouse_window_pos.x / board_square_size), 
                                    (mouse_window_pos.y / board_square_size));
    int square = (mouse_square_pos.y * GRID_SZ) + mouse_square_pos.x;

    return GRID_NUM_SQUARES - square - 1;
}

void Board::reset_square_color(int bit) {

    /* 

    Board as uin64_t where H8 is MSB 
    insert bit to reset color of.

    */

    squares[bit].setFillColor(is_square_black(bit) ? MEDIUM_BROWN : WARM_CREAM);
}

/* BITBOARD METHODS */

uint64_t Board::white_occupancy() {
    return bitboards[W_PAWNS] | bitboards[W_KNIGHTS] | bitboards[W_BISHOPS] |
           bitboards[W_ROOKS] | bitboards[W_QUEEN]   | bitboards[W_KING];
}

uint64_t Board::black_occupancy() {
    return bitboards[B_PAWNS] | bitboards[B_KNIGHTS] | bitboards[B_BISHOPS] |
           bitboards[B_ROOKS] | bitboards[B_QUEEN]   | bitboards[B_KING];
}

/* INIT */

void Board::init() {

    // Is player black or white?
    // If black, board must be inverted.
    // grid is drawn, pop up asking black or white, maybe a welcome message. not docked.
    Board::init_bitboards();
    Board::init_get_board_square_size(board_square_size, win_h, win_w);
    Board::init_main_window_squares();

    // I like this for now. Keeps it in init and only runs if debug enabled.
    if (Debug::enabled)
        Board::init_bitboard_window_squares();

    Board::init_pieces();
    // init_board_coords();
}

void Board::init_bitboards() {
    bitboards = {
        w_pawns, w_knights, w_bishops, w_rooks, w_queen, w_king,
        b_pawns, b_knights, b_bishops, b_rooks, b_queen, b_king
    };

    bitboard_names = {
        NAME_OF(w_pawns), NAME_OF(w_knights), NAME_OF(w_bishops), NAME_OF(w_rooks), NAME_OF(w_queen), NAME_OF(w_king),
        NAME_OF(b_pawns), NAME_OF(b_knights), NAME_OF(b_bishops), NAME_OF(b_rooks), NAME_OF(b_queen), NAME_OF(b_king)
    };

}

void Board::init_get_board_square_size(uint32_t& sz, const unsigned win_h, const unsigned win_w) {
    if ((win_h % GRID_SZ) != 0 | (win_w % GRID_SZ) != 0) die("Window size must support eight squares.");
    sz = win_h / GRID_SZ;
}

void Board::init_main_window_squares() {

    for (int i = H1; i <= A8; i++) {
        sf::Vector2f pos = index_to_2d(i) * (float)board_square_size;
        sf::RectangleShape rec(sf::Vector2f(board_square_size, board_square_size));
        rec.setPosition(pos);
        rec.setFillColor(is_square_black(i) ? MEDIUM_BROWN : WARM_CREAM);
        squares.insert(squares.begin(), rec);
    }
}

void Board::init_bitboard_window_squares() {

    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        sf::Vector2f pos = index_to_2d(i) * (float)board_square_size;
        bitboard_window_squares.emplace_back(sf::Vector2f(board_square_size, board_square_size));
        bitboard_window_squares[i].setPosition(pos);
    }   
}

void Board::init_pieces() {

    // if player = black_pieces
        // different starting pos.
        // prob a bitwise operation for that.

    // else        

    /* WHITE */

    // must be a way to flip this for the other board. so we dont have to rewrite.

    // if player == WHITE.
        // maybe WN_G1 - SQUARES.
    // would be good if i could give a normalised x,y and class deals with indexing etc..
    for (uint8_t i = H2; i <= A2; i++) {
        pieces.emplace_back(new Pawn("P", Color::WHITE, main_window, i, board_square_size));
    }

    pieces.emplace_back(new Knight("N", Color::WHITE, main_window, G1, board_square_size));
    pieces.emplace_back(new Knight("N", Color::WHITE, main_window, B1, board_square_size));
    
    pieces.emplace_back(new Bishop("B", Color::WHITE, main_window, F1, board_square_size));
    pieces.emplace_back(new Bishop("B", Color::WHITE, main_window, C1, board_square_size));

    pieces.emplace_back(new Rook("R", Color::WHITE, main_window, A1, board_square_size));
    pieces.emplace_back(new Rook("R", Color::WHITE, main_window, H1, board_square_size));

        
    pieces.emplace_back(new Queen("Q", Color::WHITE, main_window, D1, board_square_size));
    pieces.emplace_back(new King("K", Color::WHITE, main_window, E1, board_square_size));

    /* BLACK */

    for (uint16_t i = H7; i <= A7; i++) {
        pieces.emplace_back(new Pawn("P", Color::BLACK, main_window, i, board_square_size));
    }

    pieces.emplace_back(new Knight("N", Color::BLACK, main_window, B8, board_square_size));
    pieces.emplace_back(new Knight("N", Color::BLACK, main_window, G8, board_square_size));
    
    pieces.emplace_back(new Bishop("B", Color::BLACK, main_window, C8, board_square_size));
    pieces.emplace_back(new Bishop("B", Color::BLACK, main_window, F8, board_square_size));

    pieces.emplace_back(new Rook("R", Color::BLACK, main_window, A8, board_square_size));
    pieces.emplace_back(new Rook("R", Color::BLACK, main_window, H8, board_square_size));

    pieces.emplace_back(new Queen("Q", Color::BLACK, main_window, D8, board_square_size));
    pieces.emplace_back(new King("K", Color::BLACK, main_window, E8, board_square_size));
}

// void init_board_coords() {
//     // only need to place numbers vert and letters hor.

//     // if % 8 == 0, then its a number.

//     for (int i = 0; i < GRID_NUM_SQUARES; i++) {
//         if ()
//     }
// }

/* RENDER */

void Board::render() {
    
    render_main_window();
    if (Debug::enabled) render_bitboard_window(); 
}

void Board::render_main_window() {
    main_window.clear();
    
    // std::cout << "render_main_window()" << std::endl;

    // for (auto& squ : squares) {main_window.draw(squ);}

    for (int i = H1; i <= A8; i++) {main_window.draw(squares[i]);}

    // render_board_coords();

    for (auto& piece : pieces) {piece->draw(main_window);}
    main_window.display();
}

void Board::render_bitboard_window() {
    bitboard_window.clear();
    for (auto& squ : bitboard_window_squares) {bitboard_window.draw(squ);}
    Debug::draw_cycle_bitboard(bitboard_window, win_w, win_h, bitboards, bitboard_names, bitboard_vec_index, bitboard_window_squares);
    bitboard_window.display();
}

/* RUN */

void Board::run() {
    while (main_window.isOpen()) {
        run_handle_events();
        render();     
    }
}

/* RUN -> EVENT HANDLING */

void Board::run_handle_events() {

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

void Board::on_main_window_event(sf::Event &event) {

    if (event.type == sf::Event::Closed) main_window.close();
    if (event.type == sf::Event::KeyPressed) on_key_pressed(event); 
    if (event.type == sf::Event::MouseButtonPressed) on_mouse_press(event);   
}

void Board::on_bitboard_window_event(sf::Event &event) {
    if (event.type == sf::Event::Closed) bitboard_window.close();   
}

void Board::on_key_pressed(sf::Event &event) {

    auto key = event.key.code;

    if (key == sf::Keyboard::Tab) {
        if (!bitboard_window.isOpen() && Debug::enabled)
            // Cycle through bitboard vector and change title of bitboard_window.
            bitboard_window.create(sf::VideoMode(win_w, win_h), bitboard_names[bitboard_vec_index]);
            bitboard_vec_index = (bitboard_vec_index + 1) % bitboards.size();
            bitboard_window.setTitle(bitboard_names[bitboard_vec_index]);            
    }
}

void Board::on_mouse_press(sf::Event &event) {

    auto mouse_press = event.mouseButton.button;

    if (mouse_press == sf::Mouse::Left) {
        on_left_mouse_press();
    }
}

/* LEFT MOUSE PRESS */

void Board::on_left_mouse_press() {

    uint8_t clicked_bit = mouse_win_pos_to_bit();

    if (!selected_piece) {
        // If we dont have a selected piece, try and get one.
        selected_piece = select_piece(clicked_bit);
        return;
    }

    // We've clicked again and do have a selected piece, deselect it and its moves.
    deselect_piece();

    // If our click is not an attack then reset.
    if (!BitboardHelper::get_bit(selected_piece->attacks, clicked_bit)) {
        selected_piece = nullptr;
        return;
    }

    // Will be executed if we have a selected piece and its an attack.
    for (auto& bitboard: bitboards) {
        if (BitboardHelper::get_bit(bitboard, selected_piece->bit)){
            bitboard = BitboardHelper::clear_bit(bitboard, selected_piece->bit);
            bitboard = BitboardHelper::set_bit(bitboard, clicked_bit);

        } else if (BitboardHelper::get_bit(bitboard, clicked_bit)) {
            bitboard = BitboardHelper::clear_bit(bitboard, clicked_bit);
            Piece* piece = get_piece(clicked_bit); // another pointer to this vector. needs to be freed.
            
            // capture_piece();
            // free memory of piece perhaps and remove size of vector.
            // this works for now but still drawing presumably offscreen?
            piece->bit = 64;
            
        }

    }

    selected_piece->bit = clicked_bit;
    selected_piece = nullptr;
}

Piece* Board::select_piece(uint8_t clicked_bit) {
    
    Piece* piece = get_piece(clicked_bit);

    if (piece) {
        squares[clicked_bit].setFillColor(TURQOISE);
        piece->attacks = piece->get_legal_moves(white_occupancy(), black_occupancy());
        piece->highlight_legal_moves(piece->attacks, squares);
        return piece;
    }
            

    return nullptr;
}

void Board::deselect_piece() {

    reset_square_color(selected_piece->bit);

    for (int i = H1; i <= A8; i++) {
        if (BitboardHelper::get_bit(selected_piece->attacks, i)) {
            reset_square_color(i);
        }
    }
}

Piece* Board::get_piece(uint8_t clicked_bit) {
    for (auto& piece : pieces) {
        if (clicked_bit == piece->bit)
            return piece;
    }
}
/* OTHER MOUSE PRESS */