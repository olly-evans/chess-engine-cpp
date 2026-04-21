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

void Board::reset_square_color(sf::Vector2f square) {
    uint16_t reset_idx = pos2d_to_index(square);
    squares[reset_idx].setFillColor(is_square_black(reset_idx) ? MEDIUM_BROWN : WARM_CREAM);
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
    

    // Mindset rn, just get the 32 pieces in a vector. Add/remove class functionality as needed.


    // must be a way to flip this for the other board. so we dont have to rewrite.

    // if player == WHITE.
        // maybe WN_G1 - SQUARES.
    // would be good if i could give a normalised x,y and class deals with indexing etc..
    for (uint16_t i = WP_A2; i <= WP_H2; i++) {
        pieces.emplace_back(new Pawn("P", Color::WHITE, main_window, i, board_square_size));
    }

    pieces.emplace_back(new Knight("N", Color::WHITE, main_window, WN_G1, board_square_size));
    pieces.emplace_back(new Knight("N", Color::WHITE, main_window, WN_B1, board_square_size));
    
    pieces.emplace_back(new Bishop("B", Color::WHITE, main_window, WB_F1, board_square_size));
    pieces.emplace_back(new Bishop("B", Color::WHITE, main_window, WB_C1, board_square_size));

    pieces.emplace_back(new Rook("R", Color::WHITE, main_window, WR_A1, board_square_size));
    pieces.emplace_back(new Rook("R", Color::WHITE, main_window, WR_H1, board_square_size));

        
    pieces.emplace_back(new Queen("Q", Color::WHITE, main_window, WQ_D1, board_square_size));
    pieces.emplace_back(new King("K", Color::WHITE, main_window, WK_E1, board_square_size));

    /* BLACK */

    for (uint16_t i = BP_A7; i <= BP_H7; i++) {
        pieces.emplace_back(new Pawn("P", Color::BLACK, main_window, i, board_square_size));
    }

    pieces.emplace_back(new Knight("N", Color::BLACK, main_window, BN_B8, board_square_size));
    pieces.emplace_back(new Knight("N", Color::BLACK, main_window, BN_G8, board_square_size));
    
    pieces.emplace_back(new Bishop("B", Color::BLACK, main_window, BB_C8, board_square_size));
    pieces.emplace_back(new Bishop("B", Color::BLACK, main_window, BB_F8, board_square_size));

    pieces.emplace_back(new Rook("R", Color::BLACK, main_window, BR_A8, board_square_size));
    pieces.emplace_back(new Rook("R", Color::BLACK, main_window, BR_H8, board_square_size));

    pieces.emplace_back(new Queen("Q", Color::BLACK, main_window, BQ_D8, board_square_size));
    pieces.emplace_back(new King("K", Color::BLACK, main_window, BK_E8, board_square_size));

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

        

        // clicked_piece(); {
        int square_index = mouse_win_pos_to_square_index();
        sf::Vector2f clicked_pos = index_to_2d(square_index);

        std::cout << clicked_pos.x << ", " << clicked_pos.y << "\n";

        if (highlighted_piece) {
            // reset_highlight() {
            //}
            std::cout << highlighted_piece->legal_moves.size() << "\n";

            reset_square_color(highlighted_piece->pos);
            // reset_square_color(moves) no clue tbh but needs to happen.
            for (int i = 0; i < highlighted_piece->legal_moves.size(); i++) {
                reset_square_color(highlighted_piece->legal_moves.at(i));
            }
            highlighted_piece = nullptr;
            //click on move then move highlighted_piece->set_pos(move)
        }

        for (auto& piece : pieces) {
            // and player is white.
            if (is_vecs_equal(piece->pos, clicked_pos)) {

                // perhaps should be board function.
                piece->render_highlight(clicked_pos, squares);
                piece->set_legal_moves(white_occupancy(), black_occupancy());

                piece->highlight_legal_moves(piece->legal_moves, squares);
                highlighted_piece = piece;
            }
        }


        // }

    }
}