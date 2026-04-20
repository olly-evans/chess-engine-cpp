#include "util.hpp"
#include "pieces.hpp"
#include "board.hpp"

#include <iostream>
#include <filesystem>
#include <cstdint>

std::string Piece::resolve_texture_path() {

    // Change assets to 160px when bothered. In folder.
    std::filesystem::path path = std::filesystem::current_path();
    std::string color_prefix = (color == Color::WHITE ? "w" : "b");
    return path.string() + "/assets/" + color_prefix + piece_id + ".png";
}

Piece::Piece(std::string id, Color col, sf::RenderWindow& w, uint64_t bitb, int b_squ_sz) : 
    piece_id(std::move(id)), 
    color(col),
    window(w), 
    bitboard(bitb),
    board_square_size(b_squ_sz) {
        
    init_piece_positions_vector_from_bitboard(bitboard, piece_positions);
    if (texture.loadFromFile(get_texture_path())) {
        sprite.setTexture(texture);

        sf::FloatRect sprite_size = sprite.getGlobalBounds();
        sprite.setScale(board_square_size / sprite_size.width, 
                        board_square_size / sprite_size.height);
    }
}

std::string Piece::get_texture_path() {
    return resolve_texture_path();
}

void Piece::draw(sf::RenderWindow& window) {
    /*
        For a piece type (i.e White Rook or Black Bishop), draw all active pieces
        on the board in their appropriate position.
    */

    for (const auto& pos : piece_positions) {
        sprite.setPosition(pos.vec.x, pos.vec.y);
        window.draw(sprite);
    }
}

void Piece::init_piece_positions_vector_from_bitboard(uint64_t bitboard, std::vector<Pos> &piece_pos) {

    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        if (bitboard & (1ULL << i)) {
            int draw_idx = GRID_NUM_SQUARES - i - 1;
            sf::Vector2f pos = index_to_2d(draw_idx) * (float)board_square_size;
            Pos piece_data = {pos, false};
            // mouses pos coords not squares on grid coords.

            piece_pos.push_back(piece_data);
        }
    }
}

void Piece::render_highlight(sf::Vector2f piece_pos, std::vector<sf::RectangleShape>& squares, bool& piece_highlight_active) {
    
    for (int i = 0; i < piece_positions.size(); i++) {

        sf::Vector2f stored_pos = piece_positions[i].vec;

        stored_pos.x = stored_pos.x / board_square_size;
        stored_pos.y = stored_pos.y / board_square_size;
        
        if (!piece_highlight_active && is_vecs_equal(stored_pos, piece_pos)) {
            piece_highlight_active = true;
            int index = pos2d_to_index(piece_pos);
            squares[index].setFillColor(TURQOISE);
            this->piece_positions[i].is_highlighted = true;
        }
            
    }
}

/* PAWN */

void Pawn::get_legal_moves(Pos pos) {};

/* KNIGHT */

bool Knight::is_knight_move_on_board(Pos piece_square, int move_dx, int move_dy) {
    int new_x = piece_square.vec.x + move_dx;
    int new_y = piece_square.vec.y + move_dy;
    return (new_x >= 0 && new_x < GRID_SZ && new_y >= 0 && new_y < GRID_SZ); 
}

bool Piece::move_has_friendly_piece(Pos piece_pos, sf::Vector2f move) {
    return true;
}

void Knight::get_legal_moves(Pos piece_pos) {

    int num_offsets = sizeof(Knight::offsets) / sizeof(Knight::offsets[0]);
    
    for (int i = 0; i < num_offsets; i++) {

        int new_x = piece_pos.vec.x + offsets[i][0];
        int new_y = piece_pos.vec.y + offsets[i][1];
        sf::Vector2f move(new_x, new_y);

        if (!is_knight_move_on_board(piece_pos, Knight::offsets[i][0], Knight::offsets[i][1])) continue;
        if (move_has_friendly_piece(piece_pos, move)) continue;
        // has_friendly_piece();
            // pass piece in here, is piece on square color ==

            // then we can fill the square.
    }        
    }

/* BISHOP */

void Bishop::get_legal_moves(Pos pos) {};

/* ROOK */

void Rook::get_legal_moves(Pos pos) {};

/* QUEEN */
void Queen::get_legal_moves(Pos pos) {};

/* KING */

void King::get_legal_moves(Pos pos) {};