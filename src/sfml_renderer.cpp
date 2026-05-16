#include "sfml_renderer.hpp"
#include "sfml_event_handler.hpp"

#include "bitboardhelper.hpp"


SFMLRenderer::SFMLRenderer(const uint16_t w_width) :
    win_w(w_width), 
    main_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_WIDTH), WINDOW_NAME),
    event_handler(get_main_window()),
    board(board_square_size)
    {};

bool SFMLRenderer::is_square_black(uint8_t i) {
    uint8_t x = i % GRID_SZ;
    uint8_t y = i / GRID_SZ;
    return (x + y) % 2;
}

/* INIT */

void SFMLRenderer::init_renderer() {
    set_board_square_size(board_square_size);
    set_main_window_squares();

    board.init();
    
}

void SFMLRenderer::set_board_square_size(uint16_t& sz) {
    if ((win_w % GRID_SZ) != 0 | (win_w % GRID_SZ) != 0)    
        exit(1);

    sz = win_w / GRID_SZ;
}

uint16_t SFMLRenderer::get_board_square_size() {
    return board_square_size;
}

void SFMLRenderer::set_main_window_squares() {

    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        sf::Vector2f normalised_pos(i % GRID_SZ, i / GRID_SZ);
        sf::Vector2f pos = normalised_pos * (float)board_square_size;
        
        sf::RectangleShape rec(sf::Vector2f(board_square_size, board_square_size));

        rec.setPosition(pos);
        rec.setFillColor(is_square_black(i) ? MEDIUM_BROWN : WARM_CREAM);
        squares.insert(squares.begin(), rec);
    }
}

sf::RenderWindow& SFMLRenderer::get_main_window() {
    return main_window;
}

/* MAIN GAME LOOP */

void SFMLRenderer::run() {
    
    while (main_window.isOpen()) {
        event_handler.handle_events();
        render();
    }
}

void SFMLRenderer::render() {
    render_main_window();
}

void SFMLRenderer::render_main_window() {

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

    for (auto& piece : board.pieces) {
        uint8_t square = BBHelper::bit_to_square(piece->bit);
        sf::Vector2f normalised_pos(square % GRID_SZ, square / GRID_SZ);
        sf::Vector2f pos = normalised_pos * (float)board_square_size;
        piece->sprite.setPosition(pos.x, pos.y);
        main_window.draw(piece->sprite);
    }

    main_window.display();
}

void SFMLRenderer::render_move_highlights() {

    /* Renders turqoise circles to the square the selected piece can move. */
    
    float radius_percent_of_squares = 0.2; // Use this to change radius, care as its radius not diameter.
    float radius = board_square_size * radius_percent_of_squares;

    sf::Color circle_color = TURQOISE;

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
        circle.setFillColor(circle_color);

        main_window.draw(circle);
    }
}

void SFMLRenderer::render_capture_highlights() {
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        if (BBHelper::get_bit(selected_piece->captures, i)) 
            squares[i].setFillColor(TURQOISE);
    }
}