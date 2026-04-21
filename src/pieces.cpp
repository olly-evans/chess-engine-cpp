#include "util.hpp"
#include "pieces.hpp"
#include "board.hpp"

#include <iostream>
#include <filesystem>
#include <cstdint>

std::string Piece::resolve_texture_path() {
    std::filesystem::path path = std::filesystem::current_path();
    std::string color_prefix = (color == Color::WHITE ? "w" : "b");
    return path.string() + "/assets/" + color_prefix + piece_id + ".png";
}

Piece::Piece(std::string id, Color col, sf::RenderWindow& w, uint16_t squ_idx, int b_squ_sz) : 
    piece_id(std::move(id)), 
    color(col),
    window(w), 
    start_square_index(squ_idx),
    board_square_size(b_squ_sz) {
    

    // take squ_idx and convert to 2d normalised pos.
    pos = index_to_2d(start_square_index);

    // we then never touch index again.
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

    sf::Vector2f window_pos = this->pos * (float)board_square_size;
    sprite.setPosition(window_pos.x, window_pos.y);
    window.draw(sprite);
}

void Piece::render_highlight(sf::Vector2f clicked_pos, std::vector<sf::RectangleShape>& squares, bool& piece_highlight_active) {
    
    if (!piece_highlight_active && is_vecs_equal(clicked_pos, this->pos)) {
        piece_highlight_active = true;
        int index = pos2d_to_index(this->pos);
        squares[index].setFillColor(TURQOISE);
    }
}

// bool Piece::move_square_has_friendly_piece(Pos piece_pos, int dx, int dy) {
//     // Pos gunna need the color.
// }

/* PAWN */

void Pawn::get_legal_moves() {};

/* KNIGHT */

bool Knight::is_knight_move_on_board(sf::Vector2f piece_pos, int move_dx, int move_dy) {
    int new_x = piece_pos.x + move_dx;
    int new_y = piece_pos.y + move_dy;
    return (new_x >= 0 && new_x < GRID_SZ && new_y >= 0 && new_y < GRID_SZ); 
}

void Knight::get_legal_moves() {

    // int num_offsets = sizeof(Knight::offsets) / sizeof(Knight::offsets[0]);
    
    // for (int i = 0; i < num_offsets; i++) {

    //     int new_x = piece_pos.vec.x + Knight::offsets[i][0];
    //     int new_y = piece_pos.vec.y + Knight::offsets[i][1];
    //     sf::Vector2f move(new_x, new_y);

    //     if (!is_knight_move_on_board(piece_pos, Knight::offsets[i][0], Knight::offsets[i][1])) continue;
    //     // if (move_square_has_friendly_piece(piece_pos, move)) continue;
    //         // pass piece in here, is piece on square color ==

    //         // then we can fill the square.
    // }        
}

/* BISHOP */

void Bishop::get_legal_moves() {};

/* ROOK */

void Rook::get_legal_moves() {};

/* QUEEN */
void Queen::get_legal_moves() {};

/* KING */

void King::get_legal_moves() {};