#include "sfml_event_handler.hpp"
#include "sfml_renderer.hpp"
#include "bitboardhelper.hpp"

#include "SFML/Graphics.hpp"

/* Handles SFML events. A member of SFMLRenderer. */

SFMLEventHandler::SFMLEventHandler(Board& board, sf::RenderWindow& main_win) : 
    board(board),    
    main_window(main_win) 
    {};

int SFMLEventHandler::mouse_win_pos_to_bit() {
    // May need to be broken up into smaller functions.
    sf::Vector2i mouse_window_pos = sf::Mouse::getPosition(main_window);
    sf::Vector2i mouse_square_pos((mouse_window_pos.x / board_square_size),
                                    (mouse_window_pos.y / board_square_size));
    int square = (mouse_square_pos.y * GRID_SZ) + mouse_square_pos.x;
    return BBHelper::square_to_bit(square);
}

void SFMLEventHandler::handle_events() {

    sf::Event event;
    if (main_window.waitEvent(event)) {
        on_main_window_event(event);
    }

}

void SFMLEventHandler::on_main_window_event(sf::Event &event) {

    if (event.type == sf::Event::Closed) main_window.close();
    if (event.type == sf::Event::KeyPressed) on_key_pressed(event);
    if (event.type == sf::Event::MouseButtonPressed) on_mouse_press(event);
}


/* KEYPRESSES */

void SFMLEventHandler::on_key_pressed(sf::Event &event) {

    auto key = event.key.code;

    switch (key) {
        // case sf::Keyboard::Tab:

        //     // this'll get fucked by event and renderer but i dont care.
        //     if (!bitboard_window.isOpen() && Debug::enabled) bitboard_window.create(sf::VideoMode(win_w, win_h), bitboard_names[bitboard_vec_index]);
        //     bitboard_vec_index = (bitboard_vec_index + 1) % bitboards.size();
        //     bitboard_window.setTitle(bitboard_names[bitboard_vec_index]);
        //     break;
        // case sf::Keyboard::Z:
        //     if (Debug::enabled) {
        //         undo_move();
        //         is_whites_turn = !is_whites_turn;
        //     }
        default: 
            return;
    }   
}

/* MOUSE PRESSES */

void SFMLEventHandler::on_mouse_press(sf::Event &event) {

    auto mouse_press = event.mouseButton.button;

    switch (mouse_press) {
        case sf::Mouse::Left:
            on_left_mouse_press();
            break;
    }
}

void SFMLEventHandler::on_left_mouse_press() {

    /* 
     * Handles left mouse presses wowza!
     *
     * If we return early in this function it essentially just means wait until next left mouse press. 
     * 
     */

    uint8_t clicked_bit = mouse_win_pos_to_bit();

    if (!board.selected_piece) {
        board.selected_piece = board.select_piece(clicked_bit);
        return;
    }

    // If our click is not an move/capture then go again/reset.
    if (!BBHelper::get_bit(board.selected_piece->moves, clicked_bit) && !BBHelper::get_bit(board.selected_piece->captures, clicked_bit)) {

        // Let user select a new piece without clicking to reset.
        reset_move_and_capture_highlights(board.selected_piece->bit);
        board.selected_piece = board.select_piece(clicked_bit); // Can be null which is fine ofc.
        return;
    }

    // Below be executed if we have a selected piece and click on a valid move/capture square.
    uint8_t old_bit = board.selected_piece->bit;

    board.handle_piece_move(clicked_bit);
    reset_move_and_capture_highlights(old_bit);    
    
    // MoveLogger::show_algebraic_move_history();

    board.is_whites_turn = !board.is_whites_turn;
}

// void SFMLEventHandler::reset_move_and_capture_highlights(uint8_t selected_bit) {

//     squares[selected_bit].setFillColor(is_square_black(selected_bit) ? MEDIUM_BROWN : WARM_CREAM);
    
//     for (int i = 0; i < GRID_NUM_SQUARES; i++) {
//         if (!BBHelper::get_bit(selected_piece->captures, i)) continue;
//         squares[i].setFillColor(is_square_black(i) ? MEDIUM_BROWN : WARM_CREAM);
//     }
//     selected_piece = nullptr;
// }