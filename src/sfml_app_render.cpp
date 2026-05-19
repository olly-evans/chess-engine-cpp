#include "sfml_app.hpp"
#include "bitboardhelper.hpp"


SFMLApp::SFMLApp(Board& board, const uint16_t w_width) :
    board(board),
    win_w(w_width), 
    main_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_WIDTH), WINDOW_NAME)
    {};

bool SFMLApp::is_square_black(uint8_t i) {
    uint8_t x = i % GRID_SZ;
    uint8_t y = i / GRID_SZ;
    return (x + y) % 2;
}

/* INIT */

void SFMLApp::init() {
    
    board.init();

    set_board_square_size(board_square_size);
    set_main_window_squares();

    load_textures();
    init_piece_texture_cache(); // tmp, will be dodgy when we remove pieces.
    // will aslo fail with undo_moves()

}

void SFMLApp::set_board_square_size(uint16_t& sz) {
    if ((win_w % GRID_SZ) != 0 | (win_w % GRID_SZ) != 0)    
        exit(1);

    sz = win_w / GRID_SZ;
}

uint16_t SFMLApp::get_board_square_size() {
    return board_square_size;
}

void SFMLApp::set_main_window_squares() {

    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        sf::Vector2f normalised_pos(i % GRID_SZ, i / GRID_SZ);
        sf::Vector2f pos = normalised_pos * (float)board_square_size;
        
        sf::RectangleShape rec(sf::Vector2f(board_square_size, board_square_size));

        rec.setPosition(pos);
        rec.setFillColor(is_square_black(i) ? MEDIUM_BROWN : WARM_CREAM);
        squares.insert(squares.begin(), rec);
    }
}

void SFMLApp::load_textures() {
    for (char id : {'r','n','b','q','k','p','R','N','B','Q','K','P'}) {
        sf::Texture tex;
        if (tex.loadFromFile(resolve_texture_path(id))) {
            texture_cache[id] = std::move(tex);
        } else {
            std::cerr << "Failed to load texture for: " << id << std::endl;
        }
    }
}

std::string SFMLApp::resolve_texture_path(char id) {
    std::filesystem::path path = std::filesystem::current_path();
    std::string color_prefix = (isupper(id) ? "w" : "b");
    char tmp_id = toupper(id);
    return path.string() + "/assets/" + color_prefix + tmp_id + ".png";
}

void SFMLApp::init_piece_texture_cache() {

    // this isnt gunna work when we removed pieces.
    for (auto& piece : board.pieces) {
        if (texture_cache.count(piece->id)) {
            piece->sprite.setTexture(texture_cache[piece->id]);

            sf::FloatRect bounds = piece->sprite.getGlobalBounds();
            piece->sprite.setScale(
                get_board_square_size() / bounds.width,
                get_board_square_size() / bounds.height
            );
        }
    }
}

sf::RenderWindow& SFMLApp::get_main_window() {
    return main_window;
}

/* MAIN GAME LOOP */

void SFMLApp::run() {
    
    while (main_window.isOpen()) {
        handle_events();
        render();
    }
}

/* RENDER */

void SFMLApp::render() {
    render_main_window();
}

void SFMLApp::render_main_window() {

    /* 
    *
    *   Captures change the square color to stand out more, thus must
    *   be rendered before we redraw the squares vector to not have any
    *   delay when using waitEvent().
    * 
    */

    main_window.clear();
    if (board.selected_piece) render_capture_highlights(); // rendered before squares updates otherwise delay from waitEvent().
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        main_window.draw(squares[i]);
    }
    if (board.selected_piece) render_move_highlights();

    // render_board_coords();

    for (auto& piece : board.pieces) {
        // update_piece_pos();
        // std::cout << piece->bit << ", " << piece->id << "\n";
        uint8_t square = BBHelper::bit_to_square(piece->bit);

        sf::Vector2f normalised_pos(square % GRID_SZ, square / GRID_SZ);
        sf::Vector2f pos = normalised_pos * (float)board_square_size;
        piece->sprite.setPosition(pos.x, pos.y);
        main_window.draw(piece->sprite);
    }

    main_window.display();
}

void SFMLApp::render_move_highlights() {

    /* Renders turqoise circles to the square the selected piece can move. */
    
    float radius_percent_of_squares = 0.2; // Use this to change radius, care as its radius not diameter.
    float radius = board_square_size * radius_percent_of_squares;

    sf::Color circle_color = TURQOISE;

    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        
        // Squares indexed from A8 to H1, forgive me thus.
        uint8_t square = GRID_NUM_SQUARES - i - 1;

        if (!BBHelper::get_bit(board.selected_piece->moves, i)) continue;
        
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

void SFMLApp::render_capture_highlights() {

    // Selecting a piece means it gets a capture highlight so here we are.
    squares[board.selected_piece->bit].setFillColor(TURQOISE);

    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        if (BBHelper::get_bit(board.selected_piece->captures, i)) 
            squares[i].setFillColor(TURQOISE);
    }
}

void SFMLApp::reset_move_and_capture_highlights(uint8_t selected_bit) {

    squares[selected_bit].setFillColor(is_square_black(selected_bit) ? MEDIUM_BROWN : WARM_CREAM);
    
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        if (!BBHelper::get_bit(board.selected_piece->captures, i)) 
            continue;

        squares[i].setFillColor(is_square_black(i) ? MEDIUM_BROWN : WARM_CREAM);
    }
    board.selected_piece = nullptr;
}