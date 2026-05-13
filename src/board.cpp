#include "SFML/Graphics.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Graphics/Image.hpp"

#include "pieces.hpp"
#include "debug.hpp"
#include "bitboardhelper.hpp"
#include "player.hpp"
#include "board.hpp"
#include "gamestate.hpp"
#include "movelogger.hpp"

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


std::vector<uint64_t> Board::bitboards;

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
    return BBHelper::square_to_bit(square);
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

uint64_t Board::white_captures() {
    
    uint64_t white_captures = 0ULL;

    // this shouldnt work because captures are only updated upon calling get_pseudo_legal_moves();
    for (auto& piece : pieces) {
        if (islower(piece->piece_id))
            continue;
        white_captures |= piece->captures;
    } 
    return white_captures;
}

uint64_t Board::black_captures() {
    
    uint64_t black_captures = 0ULL;

    // this shouldnt work because captures are only updated upon calling get_pseudo_legal_moves();
    for (auto& piece : pieces) {
        if (isupper(piece->piece_id))
            continue;
        black_captures |= piece->captures;
    } 
    return black_captures;
}

// bool Board::is_piece_pinned(Piece* p) {

//     uint64_t piece = (1ULL << p->bit);
//     uint64_t enemy_captures = 0ULL;

//     bool is_white = (p->color == Color::WHITE);

//     // for every piece of opposite color get their captures.
//     uint64_t white_occ;
//     uint64_t black_occ;

//     // passing in white_occ/black_occ without the selected_piece.
//     // either genius or completely retarded.

//     // okay we do this but for every move including bit.
//     if (is_white) {
//         white_occ = white_occupancy() & ~piece;
//         black_occ = black_occupancy();
//     } else {
//         black_occ = black_occupancy() & ~piece;
//         white_occ = white_occupancy();
//     }

//     update_all_piece_moves_captures();

//     // loop through moves and ~move_bit with occupancy, calculate enemy captures if we were on move bit.
//     for (auto& piece : pieces) {

//         if ((piece->color == p->color))
//             continue;
        
        
//         // feels expensive but its not too bad.
//         // must update this->captures.

//         piece->get_pseudo_legal_moves(white_occ, black_occ);

//         enemy_captures |= piece->captures;
//     }

//     uint64_t friendly_king = (is_white) ? bitboards[W_KING] : bitboards[B_KING];

//     if (friendly_king & enemy_captures)
//         return true;
//     return false;
// }

bool Board::white_king_in_check(uint64_t white, uint64_t black) {

    uint64_t king = bitboards[W_KING];
    uint64_t enemy_captures = 0ULL;

    for (auto& piece : pieces) {

        if ((piece->color == Color::WHITE))
            continue;
        
        // feels expensive but its not too bad.
        // must update this->captures.
        piece->get_pseudo_legal_moves(white_occupancy(), black_occupancy());

        enemy_captures |= piece->captures;
    }

    if (enemy_captures & king)
        return true;

    return false;
}

bool Board::black_king_in_check(uint64_t white, uint64_t black) {

    uint64_t king = bitboards[B_KING];
    uint64_t enemy_captures = 0ULL;

    for (auto& piece : pieces) {

        if ((piece->color == Color::BLACK))
            continue;
        
        // must update this->captures.
        piece->get_pseudo_legal_moves(white_occupancy(), black_occupancy());

        enemy_captures |= piece->captures;
    }

    if (enemy_captures & king)
        return true;
    return false;
}


void Board::update_all_piece_moves_captures() {

    for (auto& piece : pieces) {
        piece->get_pseudo_legal_moves(white_occupancy(), black_occupancy());
    }

}

/* INIT */

void Board::init() {


    // Selection window with empty colored squares to choose white/black.
    
    // this kinda doesn't matter right now.
    Board::init_players();

    // okey so in here somehwere we can find the players and see if we need to flip the board
    // graphically. bitboards const.

    // Board::init_bitboards_from_fen(fen);
    bitboards = {
        w_pawns, w_knights, w_bishops, w_rooks, w_queen, w_king,
        b_pawns, b_knights, b_bishops, b_rooks, b_queen, b_king
    };

    bitboard_names = {
        'P', 'N', 'B', 'R', 'Q', 'K',
        'p', 'n', 'b', 'r', 'q', 'k',
    };


    Board::init_get_board_square_size(board_square_size, win_h, win_w);
    // init_board_coords();

    // for now i wont flip the board if we have two human players.
    // so for now we'll assume one player is an Engine but as a Human.

    init_main_window_squares();

    // I like this for now. Keeps it in init and only runs if debug enabled.
    if (Debug::enabled) Board::init_bitboard_window_squares();

    // What needs to happen if fen string is invalid.

    std::string fen = "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2";
    // std::string fen = "8/8/8/4k3/8/4P3/4K3/8 w - - 0 1";
    init_position_from_fen(fen);
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

// this can be done with one function for both functions below.
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

    /* Parses fen string and appropriately initialises bitboards. */

    std::vector<std::string> fen_tokens = FenParser::split_with_delimiter(fen, " ");


    // I want these hardcoded tokens to have seperate functions.
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

            create_piece(ch, bit);
            file++;

            // Get correct piece type bitboard from ch.
            uint64_t& bitboard = FenParser::get_fen_char_bitboard(ch, bitboards);
            BBHelper::set_bit_by_ref(bitboard, bit);
        }
    }

    is_whites_turn = (fen_tokens[1] == "w");

    // Parse more tokens later if we want to.
}

void Board::create_piece(const char id, uint8_t bit) {

    enum Color color = (isupper(id)) ? Color::WHITE : Color::BLACK;
    const char norm_id = toupper(id);

    switch (norm_id) {

        case 'P':
            pieces.emplace_back(new Pawn(id, color, main_window, bit, board_square_size));
            break;
        case 'N':
            pieces.emplace_back(new Knight(id, color, main_window, bit, board_square_size));
            break;
        case 'B':
            pieces.emplace_back(new Bishop(id, color, main_window, bit, board_square_size));
            break;
        case 'R':
            pieces.emplace_back(new Rook(id, color, main_window, bit, board_square_size));
            break;
        case 'Q':
            pieces.emplace_back(new Queen(id, color, main_window, bit, board_square_size));
            break;
        case 'K':
            pieces.emplace_back(new King(id, color, main_window, bit, board_square_size));
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

// I'm thinking render belongs in its own module. With as much sfml logic as possible.
void Board::render() {
    render_main_window();
    if (Debug::enabled) render_bitboard_window();
}

void Board::render_main_window() {

    /* 
    *
    *   Captures change the square color to stand out more, thus must
    *   be rendered before we redraw the squares vector to not have any
    *   delay when using waitEvent().
    * 
    */

    main_window.clear();
    if (selected_piece) render_capture_highlights();
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {main_window.draw(squares[i]);}
    if (selected_piece) render_move_highlights();

    // render_board_coords();

    for (auto& piece : pieces) {piece->draw(main_window);}
    main_window.display();
}

void Board::render_move_highlights() {

    /* Renders turqoise circles to the square the selected piece can move. */
    
    float radius_percent_of_squares = 0.2; // Use this to change radius, care as its radius not diameter.
    float radius = board_square_size * radius_percent_of_squares;
    

    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        
        // Squares indexed from A8 to H1, forgive me thus.
        uint8_t square = GRID_NUM_SQUARES - i - 1;

        if (!BBHelper::get_bit(selected_piece->moves, i)) continue;
        
        sf::Vector2f normalised_pos(square % GRID_SZ, square / GRID_SZ);
        sf::Vector2f pos = normalised_pos * (float)board_square_size;

        // Offset to centre circle in square. Top left placed at pos.
        pos.x += ((float)board_square_size / 2) - radius;
        pos.y += ((float)board_square_size / 2) - radius;

        sf::CircleShape circle;
        circle.setPosition(pos);
        circle.setRadius(radius);
        circle.setFillColor(TURQOISE);

        main_window.draw(circle);
    }
}

void Board::render_capture_highlights() {
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        if (BBHelper::get_bit(selected_piece->captures, i)) squares[i].setFillColor(TURQOISE);
    }
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
    free_pieces(); 
}

/* RUN -> EVENT HANDLING */

void Board::handle_events() {

    sf::Event event;

    if (main_window.waitEvent(event)) {
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
        case sf::Keyboard::Z:
            if (Debug::enabled) {
                undo_move();
                is_whites_turn = !is_whites_turn;
            }
        default: 
            return;
    }   
}

void Board::undo_move() {

    if (MoveLogger::move_history.empty()) 
        return;

    Move& last_move = MoveLogger::move_history.back();

    // Just so if we undo a move whilst user has piece selected we don't get funny business.
    if (selected_piece) 
        reset_move_and_capture_highlights(selected_piece->bit);

    uint64_t& moved_piece_bitboard = FenParser::get_fen_char_bitboard(last_move.moved_id, bitboards);

    BBHelper::set_bit_by_ref(moved_piece_bitboard, last_move.start_bit);
    BBHelper::clear_bit_by_ref(moved_piece_bitboard, last_move.end_bit);
    
    Piece* moved_piece = get_piece(last_move.end_bit);
    moved_piece->bit = last_move.start_bit; 

    // If the move didn't involve a capture we can clean up and return early.
    if (!last_move.has_capture) {
        MoveLogger::move_history.pop_back(); 
        return;
    }

    // capture_bit changes depending on move type in handle_piece_move.
    create_piece(last_move.captured_id, last_move.capture_bit);
    uint64_t& captured_piece_bitboard = FenParser::get_fen_char_bitboard(last_move.captured_id, bitboards);
    BBHelper::set_bit_by_ref(captured_piece_bitboard, last_move.capture_bit);

    MoveLogger::move_history.pop_back();
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

    /* 
     * Handles left mouse presses wowza!
     *
     * If we return early in this function it essentially just means wait until next left mouse press. 
     * 
     */

    uint8_t clicked_bit = mouse_win_pos_to_bit();

    if (!selected_piece) {
        selected_piece = select_piece(clicked_bit);
        return;
    }

    // If our click is not an move/capture then go again/reset.
    if (!BBHelper::get_bit(selected_piece->moves, clicked_bit) && !BBHelper::get_bit(selected_piece->captures, clicked_bit)) {

        // Let user select a new piece without clicking to reset.
        reset_move_and_capture_highlights(selected_piece->bit);
        selected_piece = select_piece(clicked_bit); // Can be null which is fine ofc.
        return;
    }

    // Below be executed if we have a selected piece and click on a valid move/capture square.
    uint8_t old_bit = selected_piece->bit;

    handle_piece_move(clicked_bit);
    reset_move_and_capture_highlights(old_bit);    
    
    // MoveLogger::show_algebraic_move_history();

    is_whites_turn = !is_whites_turn;
}

Piece* Board::select_piece(uint8_t clicked_bit) {

    Piece* piece = get_piece(clicked_bit);
    if (!piece) return nullptr;

    // TODO:
    // Works fine but will eventually be a dependancy nightmare, perhaps move checks to gamestate class.
    // bool can_white_move()
    // bool can_black_move()
    
    if (piece->color == Color::BLACK && is_whites_turn) return nullptr;
    if (piece->color == Color::WHITE && !is_whites_turn) return nullptr;

    // Highlight the square they clicked on.
    squares[clicked_bit].setFillColor(TURQOISE);

    piece->moves = piece->get_pseudo_legal_moves(white_occupancy(), black_occupancy());
    // set_pseudo_legal_attacks(); void we dont even use piece->moves after in this function.

    piece->remove_pseudo_legal_moves(*this);

    return piece;
}


uint64_t Board::get_white_captures(uint64_t white, uint64_t black) {

    uint64_t white_captures = 0ULL;
    for (auto& piece : pieces) {

            if (piece->color != Color::WHITE)
                continue;
            
            // This just means if we pass in a pseudo white/black occupancy we can ignore certain captures.
            if (!(white & (1ULL << piece->bit)))
                continue;

            piece->get_pseudo_legal_moves(white, black);

            white_captures |= piece->captures;
        }
    return white_captures;
}

uint64_t Board::get_black_captures(uint64_t white, uint64_t black) {

    uint64_t black_captures = 0ULL;
    for (auto& piece : pieces) {

            if (piece->color != Color::BLACK)
                continue;
            
            // This just means if we pass a pseudo white/black occupancy we can ignore certain captures.
            if (!(black & (1ULL << piece->bit)))
                continue;

            piece->get_pseudo_legal_moves(white, black);

            black_captures |= piece->captures;
        }
    return black_captures;
}

void Board::reset_move_and_capture_highlights(uint8_t selected_bit) {

    squares[selected_bit].setFillColor(is_square_black(selected_bit) ? MEDIUM_BROWN : WARM_CREAM);
    
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        if (!BBHelper::get_bit(selected_piece->captures, i)) continue;
        squares[i].setFillColor(is_square_black(i) ? MEDIUM_BROWN : WARM_CREAM);
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

bool Board::bit_has_piece(uint8_t clicked_bit) {
    
    for (auto& bitboard : bitboards) {
        if (bitboard & (1ULL << clicked_bit)) return true;
    }
    return false;
}

void Board::remove_piece(uint8_t piece_to_remove_bit) {

    auto it = std::find_if(pieces.begin(), pieces.end(), [piece_to_remove_bit](Piece* p) {
        return p->bit == piece_to_remove_bit;
    });

    if (it != pieces.end()) {
        delete *it;        // free the memory
        pieces.erase(it);  // remove from vector
    }
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

    /* Log Move */

    // prob dont need selected bit and that if passing in whole board.
    MoveLogger::log_move(*this, clicked_bit, selected_piece->bit, selected_piece->piece_id);

    /* Process clicked_bit into a move */
    
    // White moving up, black moves down. Capture bit for EP is clicked_bit +-8 bits depending on color.
    uint8_t ep_capture_bit = (selected_piece->color == Color::WHITE) ? clicked_bit - 8 : clicked_bit + 8;

    for (auto& bitboard: bitboards) {

        // works but can we just check this once.
        // goes first because otherwise we move the piece before checking if enpassant.
        if (is_enpassant_capture(clicked_bit)) {

            bitboard = BBHelper::clear_bit(bitboard, ep_capture_bit);
            remove_piece(ep_capture_bit);
        } else if (BBHelper::get_bit(bitboard, selected_piece->bit)){

            // No capture, just a move.
            bitboard = BBHelper::clear_bit(bitboard, selected_piece->bit);
            bitboard = BBHelper::set_bit(bitboard, clicked_bit);
        } else if (BBHelper::get_bit(bitboard, clicked_bit)) {

            // Normal capture.
            bitboard = BBHelper::clear_bit(bitboard, clicked_bit);
            remove_piece(clicked_bit);
        } 
    }

    selected_piece->bit = clicked_bit;
}

bool Board::is_enpassant_capture(uint8_t clicked_bit) {

    Pawn* pawn = dynamic_cast<Pawn*>(selected_piece);

    if (!pawn) 
        return false;

    if (bit_has_piece(clicked_bit))
        return false;

    uint8_t color_ep_offset = (pawn->color == Color::WHITE) ? -8 : 8;
    uint8_t ep_capture_bit = clicked_bit + color_ep_offset;

    if (!bit_has_piece(ep_capture_bit))
        return false;

    if (!(pawn->en_passant_captures & (1ULL << (ep_capture_bit))))
        return false;

    /* 
       Get to here and: we have a pawn, there is no piece on clicked_bit, 
       there is a piece on ep_capture_bit and ep_capture_bit is a 
       valid en passant capture.
    */

    return true;  
}

/* FREES */

void Board::free_pieces() {
    for (auto& piece : pieces) {delete piece;}
}

