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

Piece::Piece(std::string id, Color col, sf::RenderWindow& w, uint16_t b, int b_squ_sz) : 
    piece_id(std::move(id)), 
    color(col),
    window(w), 
    bit(b),
    board_square_size(b_squ_sz) {
        
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

    uint8_t square = BitboardHelper::bit_to_square(this->bit);
    sf::Vector2f pos = index_to_2d(square) * (float)board_square_size;
    sprite.setPosition(pos.x, pos.y);
    window.draw(sprite);
}

// void Piece::render_highlight(uint8_t clicked_bit, std::vector<sf::RectangleShape>& squares) {
    
//     if (clicked_bit == this->bit) {
//         squares[clicked_bit].setFillColor(TURQOISE);
//     }
// }

void Piece::highlight_legal_moves(uint64_t attacks, std::vector<sf::RectangleShape>& squares) {
    
    // find bits that are 1.

    for (int bit = 0; bit < GRID_NUM_SQUARES; bit++) {

        if (attacks & (1ULL << bit)) {
            squares[bit].setFillColor(TURQOISE);
        }
    }
}

/* PAWN */

uint64_t Pawn::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* KNIGHT */

bool Knight::is_knight_move_on_board(sf::Vector2f piece_pos, int move_dx, int move_dy) {
    int new_x = piece_pos.x + move_dx;
    int new_y = piece_pos.y + move_dy;
    return (new_x >= 0 && new_x < GRID_SZ && new_y >= 0 && new_y < GRID_SZ);
}

uint64_t Knight::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {
    
    uint64_t knight = 1ULL << this->bit;
    uint64_t attacks = 0ULL;

    // gunna be interesting when we need to flip the board.

    attacks |= (knight & NOT_AB_FILE) << 6;
    attacks |= (knight & NOT_A_FILE)  << 15; 
    attacks |= (knight & NOT_H_FILE)  << 17;
    attacks |= (knight & NOT_GH_FILE) << 10;
    
    attacks |= (knight & NOT_GH_FILE) >> 6;   
    attacks |= (knight & NOT_H_FILE)  >> 15;  
    attacks |= (knight & NOT_A_FILE)  >> 17; 
    attacks |= (knight & NOT_AB_FILE) >> 10; 

    return BitboardHelper::remove_friendly_pieces(attacks, this->color == Color::WHITE ? w_bb : b_bb);
}



/* BISHOP */

uint64_t Bishop::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* ROOK */

uint64_t Rook::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* QUEEN */
uint64_t Queen::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* KING */

uint64_t King::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {};