#include "SFML/Graphics.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Graphics/Image.hpp"

#include "pieces.hpp"
#include "debug.hpp"

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
win_name) {};

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

int Board::mouse_win_pos_to_square_index() {
    // May need to be broken up into smaller functions.
    sf::Vector2i mouse_window_pos = sf::Mouse::getPosition(main_window);
    sf::Vector2i mouse_square_pos((mouse_window_pos.x / board_square_size), 
                                    (mouse_window_pos.y / board_square_size));
    return (mouse_square_pos.y * GRID_SZ) + mouse_square_pos.x;
}


int Board::square_index_to_piece_type_index(int square_index) {

    // this function might as well return piece pointer.

    int bitboard_idx = GRID_NUM_SQUARES - square_index - 1;
    for (int i = 0; i < piece_types.size(); i++) {
        // condition might be dodge.
        // input is zero here, which would be 0,0.
        if (is_bit_set(piece_types.at(i)->bitboard, bitboard_idx)) return i;
    }
    return -1;
}

// only here in case we pass board to pieces, well maybe other reasons too eventually.
const unsigned int Board::get_win_width() {
    return win_w; 
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

    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        sf::Vector2f pos = index_to_2d(i) * (float)board_square_size;
        squares.emplace_back(sf::Vector2f(board_square_size, board_square_size));
        squares[i].setPosition(pos);
        squares[i].setFillColor(is_square_black(i) ? MEDIUM_BROWN : WARM_CREAM);
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

void Board::render() {
    
    render_main_window();
    if (Debug::enabled) render_bitboard_window(); 
}

void Board::render_main_window() {
    main_window.clear();
    
    // std::cout << "render_main_window()" << std::endl;

    for (auto& squ : squares) {main_window.draw(squ);}

    // render_board_coords();

    for (auto& piece_type : piece_types) {piece_type->draw(main_window);}
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

    // IF WE HAVE A HIGHLIGHTED PIECE
    // GET ITS VECTOR FROM Pos STRUCT
    // SET IT BACK TO FALSE.
    // MAYBE

    auto mouse_press = event.mouseButton.button;

    if (mouse_press == sf::Mouse::Left) {
        // ideally all rendering is done in render().
        // but this is all rendering pretty much.
        // but it is piece methods so i guess its eh.

        
        int square_index = mouse_win_pos_to_square_index();
        uint64_t piece_index = square_index_to_piece_type_index(square_index);
        if (piece_index == -1) return;
        
        sf::Vector2f pos = index_to_2d(square_index);
        
        piece_types[piece_index]->render_highlight(pos, squares, piece_highlight_active);
        // pieces_types[piece_index]->get_legal_moves();
        // piece_types[piece_index]->render_legal_moves();


        // piece_selected();
            // .render_highlight();
            // .render_moves();
    }
}