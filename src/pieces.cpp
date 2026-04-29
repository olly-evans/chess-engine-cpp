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

Piece::Piece(std::string id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz) : 
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
    sf::Vector2f normalised_pos(square % GRID_SZ, square / GRID_SZ);
    sf::Vector2f pos = normalised_pos * (float)board_square_size;
    sprite.setPosition(pos.x, pos.y);
    window.draw(sprite);
}

void Piece::highlight_legal_moves(uint64_t attacks, std::vector<sf::RectangleShape>& squares) {
    
    for (int bit = 0; bit < GRID_NUM_SQUARES; bit++) {
        if (attacks & (1ULL << bit)) {
            squares[bit].setFillColor(TURQOISE);
        }
    }
}

/* PAWN */

uint64_t Pawn::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* KNIGHT */

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

uint64_t Rook::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {

    uint64_t rook = (1ULL << this->bit);
    uint64_t attacks = 0ULL;

    // There is only trial and error behind these masks I'll be real.

    // condition into bitboardhelper func for sure.

    // while attacks not including other pieces.

    uint64_t north_attacks = BitboardHelper::get_viable_north_attacks(rook, w_bb, b_bb);
    uint64_t south_attacks = BitboardHelper::get_viable_south_attacks(rook, w_bb, b_bb);

    attacks |= north_attacks;
    attacks |= south_attacks;
    return attacks;

//     // condition stops all directions when we see a piece.


//     while (!(w_bb & attacks) | !(b_bb & attacks)) {
//         // condition isn't met for a while, number doesnt change after first iteration.


//         // need to stop individually when condition met.
//         // and stop in direction when friendly piece hit too, not at the end.

//         // some mix of ifs and continue to stop directions at pieces i think.
        
    // get_viable_north_attacks(); // returns a bitboard then or with attacks.
    // get_viable_south_offsets(); 


//         // attacks |= (rook & NOT_H_FILE) << (hor_offset + i);
//         // attacks |= (rook << (vert_offset + (vert_offset*i)));

//         // attacks |= (rook & NOT_A_FILE) >> hor_offset + i;
//         // attacks |= (rook >> (vert_offset + (vert_offset*i)));
//    }

};

/* QUEEN */
uint64_t Queen::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* KING */

uint64_t King::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {};