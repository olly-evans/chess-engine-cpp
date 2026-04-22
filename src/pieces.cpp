#include "util.hpp"
#include "pieces.hpp"
#include "board.hpp"
#include "bitboardhelper.hpp"

#include <iostream>
#include <filesystem>
#include <cstdint>

std::string Piece::resolve_texture_path() {
    std::filesystem::path path = std::filesystem::current_path();
    std::string color_prefix = (color == Color::WHITE ? "w" : "b");
    return path.string() + "/assets/" + color_prefix + piece_id + ".png";
}

Piece::Piece(std::string id, Color col, sf::RenderWindow& w, uint16_t s_squ_idx, int b_squ_sz) : 
    piece_id(std::move(id)), 
    color(col),
    window(w), 
    start_square_index(s_squ_idx),
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

void Piece::render_highlight(sf::Vector2f clicked_pos, std::vector<sf::RectangleShape>& squares) {
    
    if (is_vecs_equal(clicked_pos, this->pos)) {
        int index = pos2d_to_index(this->pos);
        squares[index].setFillColor(TURQOISE);
    }
}

void Piece::highlight_legal_moves(uint64_t attacks, std::vector<sf::RectangleShape>& squares) {
    
    // find bits that are 1.

    for (int bit = 0; bit < GRID_NUM_SQUARES; bit++) {

        if (attacks & (1ULL << bit)) {
            int square = BitboardHelper::square_to_bit(bit);
            squares[square].setFillColor(TURQOISE);
        }
    }
}

/* PAWN */

uint64_t Pawn::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* KNIGHT */

bool Knight::is_knight_move_on_board(sf::Vector2f piece_pos, int move_dx, int move_dy) {
    int new_x = piece_pos.x + move_dx;
    int new_y = piece_pos.y + move_dy;
    return (new_x >= 0 && new_x < GRID_SZ && new_y >= 0 && new_y < GRID_SZ);
}

uint64_t Knight::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {
    
    int square = pos2d_to_index(this->pos); // 1,7 -> 57
    int bit = BitboardHelper::square_to_bit(square); // should conv 57 to 6.

    uint64_t knight = 1ULL << bit;
    uint64_t attacks = 0ULL;
    
    // MASKS.
    const uint64_t NOT_A_FILE  = 0xFEFEFEFEFEFEFEFEULL;
    const uint64_t NOT_AB_FILE = 0xFCFCFCFCFCFCFCFCULL;
    const uint64_t NOT_H_FILE  = 0x7F7F7F7F7F7F7F7FULL;
    const uint64_t NOT_GH_FILE = 0x3F3F3F3F3F3F3F3FULL;

    // gunna be interesting when we need to flip the board.
    
    attacks |= (knight & NOT_AB_FILE) << 6;   // 2 left, 1 up
    attacks |= (knight & NOT_A_FILE)  << 15;  // 1 left, 2 up  
    attacks |= (knight & NOT_H_FILE)  << 17;  // 1 right, 2 up
    attacks |= (knight & NOT_GH_FILE) << 10;  // 2 right, 1 up
    attacks |= (knight & NOT_GH_FILE) >> 6;   // 2 right, 1 down
    attacks |= (knight & NOT_H_FILE)  >> 15;  // 1 right, 2 down
    attacks |= (knight & NOT_A_FILE)  >> 17;  // 1 left, 2 down
    attacks |= (knight & NOT_AB_FILE) >> 10;  // 2 left, 1 down

    return BitboardHelper::remove_friendly_pieces(attacks, this->color == Color::WHITE ? w_bb : b_bb);
}



/* BISHOP */

uint64_t Bishop::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* ROOK */

uint64_t Rook::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* QUEEN */
uint64_t Queen::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* KING */

uint64_t King::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};