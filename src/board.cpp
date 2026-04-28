#include "SFML/Graphics.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Graphics/Image.hpp"

#include "pieces.hpp"
#include "debug.hpp"
#include "bitboardhelper.hpp"
#include "player.hpp"
#include "board.hpp"

#include <sstream>
#include <iostream>
#include <cmath>

// class AbstractBoard {
//     virtual void die(std::string) = 0;
//     
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

bool Board::is_square_black(uint8_t i) {
    uint8_t x = i % GRID_SZ;
    uint8_t y = i / GRID_SZ;
    return (x + y) % 2;
}

int Board::mouse_win_pos_to_bit() {
    // May need to be broken up into smaller functions.
    sf::Vector2i mouse_window_pos = sf::Mouse::getPosition(main_window);
    sf::Vector2i mouse_square_pos((mouse_window_pos.x / board_square_size), 
                                    (mouse_window_pos.y / board_square_size));
    int square = (mouse_square_pos.y * GRID_SZ) + mouse_square_pos.x;
    return BitboardHelper::square_to_bit(square);
}

void Board::reset_square_color(int bit) {
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

    Board::init_players();

    // TODO:
    // init_bitboards_from_fen();

    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // Board::init_bitboards_from_fen(fen);
    bitboards = {
        w_pawns, w_knights, w_bishops, w_rooks, w_queen, w_king,
        b_pawns, b_knights, b_bishops, b_rooks, b_queen, b_king
    };

    bitboard_names = {
        NAME_OF(w_pawns), NAME_OF(w_knights), NAME_OF(w_bishops), NAME_OF(w_rooks), NAME_OF(w_queen), NAME_OF(w_king),
        NAME_OF(b_pawns), NAME_OF(b_knights), NAME_OF(b_bishops), NAME_OF(b_rooks), NAME_OF(b_queen), NAME_OF(b_king)
    };


    Board::init_get_board_square_size(board_square_size, win_h, win_w);
    // init_board_coords();

    // if white_player->is_human()
        // init_main_window_squares_white();
    // else
        // init_main_window_squares_black();


    // for now i wont flip the board if we have two human players.
    // so for now we'll assume one player is an Engine but as a Human.

    init_main_window_squares();

    // I like this for now. Keeps it in init and only runs if debug enabled.
    if (Debug::enabled) Board::init_bitboard_window_squares();

    init_position_from_fen(fen);
    // init_bitboards_from_fen(fen);

}

void Board::init_players() {

    bool is_white = true; //temp

    white_player = new Human(is_white);
    black_player = new Engine(!is_white);

}

void Board::init_get_board_square_size(uint32_t& sz, const unsigned win_h, const unsigned win_w) {
    if ((win_h % GRID_SZ) != 0 | (win_w % GRID_SZ) != 0) die("Window size must support eight squares.");
    sz = win_h / GRID_SZ;
}

void Board::init_main_window_squares() {

    for (int i = H1; i <= A8; i++) {
        sf::Vector2f normalised_pos(i % GRID_SZ, i / GRID_SZ);
        sf::Vector2f pos = normalised_pos * (float)board_square_size;
        sf::RectangleShape rec(sf::Vector2f(board_square_size, board_square_size));
        rec.setPosition(pos);
        rec.setFillColor(is_square_black(i) ? MEDIUM_BROWN : WARM_CREAM);
        squares.insert(squares.begin(), rec);
    }
}

void Board::init_bitboard_window_squares() {

    // From A8-H1.
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        sf::Vector2f normalised_pos(i % GRID_SZ, i / GRID_SZ);
        sf::Vector2f pos = normalised_pos * (float)board_square_size;
        bitboard_window_squares.emplace_back(sf::Vector2f(board_square_size, board_square_size));
        bitboard_window_squares[i].setPosition(pos);
    }   
}

void Board::init_position_from_fen(std::string fen) {

    /* Parses fen string and converts position to appropriate bitboards */

    // Not ideal that the split is 

    std::vector<std::string> fen_tokens = FenParser::split_with_delimiter(fen, " ");

    std::string board = fen_tokens[0];
    int rank = 7, file = 0;
    for (char ch : board) {
        
        if (ch == '/') {
            rank--;
            file = 0;
        } else if (isdigit(ch)) {
            file += ch - '0';
        } else if (isalpha(ch)) {
            int bit = rank * 8 + (7 - file);

            FenCharInfo info = FenParser::get_fen_char_info(ch);
            create_piece(info, bit);
            file++;

            // Convert fen to piece type bitboards.
            uint64_t& bitboard = FenParser::get_fen_char_bitboard(ch, bitboards);
            BitboardHelper::set_bit_by_ref(bitboard, bit);
        }
    }

    is_whites_turn = (fen_tokens[1] == "w") ? true : false;
    
    // Parse more tokens later if we want to.
}

void Board::create_piece(const FenCharInfo& info, uint8_t bit) {

    switch (info.piece_id) {
        case 'P': 
            pieces.emplace_back(new Pawn("P", info.color, main_window, bit, board_square_size));
            break;
        case 'N': 
            pieces.emplace_back(new Knight("N", info.color, main_window, bit, board_square_size));
            break;
        case 'B': 
            pieces.emplace_back(new Bishop("B", info.color, main_window, bit, board_square_size));
            break;
        case 'R': 
            pieces.emplace_back(new Rook("R", info.color, main_window, bit, board_square_size));
            break;
        case 'Q': 
            pieces.emplace_back(new Queen("Q", info.color, main_window, bit, board_square_size));
            break;
        case 'K': 
            pieces.emplace_back(new King("K", info.color, main_window, bit, board_square_size));
            break;
        default: break;
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

void Board::render() {
    render_main_window();
    if (Debug::enabled) render_bitboard_window(); // dodge.
}

void Board::render_main_window() {
    main_window.clear();
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {main_window.draw(squares[i]);}

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
        handle_events();
        render();     
    }
}

/* RUN -> EVENT HANDLING */

void Board::handle_events() {

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

void Board::on_main_window_event(sf::Event &event) {

    if (event.type == sf::Event::Closed) main_window.close();
    if (event.type == sf::Event::KeyPressed) on_key_pressed(event); 
    if (event.type == sf::Event::MouseButtonPressed) on_mouse_press(event);   
}

void Board::on_bitboard_window_event(sf::Event &event) {
    if (event.type == sf::Event::Closed) bitboard_window.close();   
}

/* KEYPRESSES */

void Board::on_key_pressed(sf::Event &event) {

    auto key = event.key.code;

    switch (key) {
        case sf::Keyboard::Tab:
            if (!bitboard_window.isOpen() && Debug::enabled) bitboard_window.create(sf::VideoMode(win_w, win_h), bitboard_names[bitboard_vec_index]);
            bitboard_vec_index = (bitboard_vec_index + 1) % bitboards.size();
            bitboard_window.setTitle(bitboard_names[bitboard_vec_index]); 
            break;
    }
}

/* MOUSE PRESSES */

void Board::on_mouse_press(sf::Event &event) {

    auto mouse_press = event.mouseButton.button;

    switch (mouse_press) {
        case sf::Mouse::Left:
            on_left_mouse_press();
            break;
    }
}

void Board::on_left_mouse_press() {

    /* This doesn't really use Player class yet. Perhaps it doesn't need to
    *  but may be useful later.
    */

    uint8_t clicked_bit = mouse_win_pos_to_bit();

    if (!selected_piece) {
        selected_piece = select_piece(clicked_bit);
        return;
    }  
    
    // Save this bit so we can reset the color later in deselect_piece().
    // could call the square init function if this gets hairy.
    uint8_t old_bit = selected_piece->bit;

    // If our click is not an attack then go again/reset.
    if (!BitboardHelper::get_bit(selected_piece->attacks, clicked_bit)) {
        deselect_piece(old_bit);
        return;
    }

    // Will be executed if we have a selected piece and click on a valid attack square.

    // if not your turn, dont show higlights.

    if (!is_whites_turn && selected_piece->color == Color::WHITE) return;
    if (is_whites_turn && selected_piece->color == Color::BLACK) return;

    handle_piece_move(clicked_bit);
    deselect_piece(old_bit);

    is_whites_turn = !is_whites_turn;
}

Piece* Board::select_piece(uint8_t clicked_bit) {
    
    Piece* piece = get_piece(clicked_bit);
    if (!piece) return nullptr;

    // Works fine but will eventually be a dependancy nightmare, perhaps move checks to gamestate class.
    // bool can_white_move()
    // bool can_black_move()

    // If not your turn you can't select a piece.
    if (piece->color == Color::BLACK && is_whites_turn) return nullptr;
    if (piece->color == Color::WHITE && !is_whites_turn) return nullptr;

    squares[clicked_bit].setFillColor(TURQOISE);
    piece->attacks = piece->get_legal_moves(white_occupancy(), black_occupancy());
    piece->highlight_legal_moves(piece->attacks, squares);
    return piece;
}

void Board::deselect_piece(uint8_t old_bit) {

    // Old bit is bit of piece we clicked on.
    reset_square_color(old_bit);

    // Reset square color of its attacks now its not selected.
    for (int i = H1; i <= A8; i++) {
        if (BitboardHelper::get_bit(selected_piece->attacks, i)) {
            reset_square_color(i);
        }
    }
    selected_piece = nullptr;
}

Piece* Board::get_piece(uint8_t clicked_bit) {
    for (auto& piece : pieces) {
        if (clicked_bit == piece->bit)
            return piece;
    }
    return nullptr;
}


void Board::handle_piece_move(uint8_t clicked_bit) {

    /* 

        This function is a bit funny. For it to be called selected_piece
        must not be null so no check needed.
        
        Essentially we find the bitboard of the selected piece type and update 
        it.

        Find the bitboard of the clicked bit if any, clear the bitboard bit and 
        place the selected_piece there by updating its piece->bit.

        If a piece is selected we cannot attack/move to a friendly piece, so no
        need for that logic.

    */

    for (auto& bitboard: bitboards) {

        // TODO: Log the move into a move class.
        // TODO: Allow moves to be undone.

        if (BitboardHelper::get_bit(bitboard, selected_piece->bit)){
            bitboard = BitboardHelper::clear_bit(bitboard, selected_piece->bit);
            bitboard = BitboardHelper::set_bit(bitboard, clicked_bit);

        } else if (BitboardHelper::get_bit(bitboard, clicked_bit)) {            
            bitboard = BitboardHelper::clear_bit(bitboard, clicked_bit);
            
            // Find piece and remove piece from pieces vector.
            auto it = std::find_if(pieces.begin(), pieces.end(), [clicked_bit](Piece* p) {
                return p->bit == clicked_bit;
            });

            if (it != pieces.end()) {
                delete *it;        // free the memory
                pieces.erase(it);  // remove from vector
            }
        }
    }
    selected_piece->bit = clicked_bit;
}