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

/* PAWN */

uint64_t Pawn::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {


    uint64_t pawn = 1ULL << this->bit;
    uint64_t moves = 0ULL;

    
    if (this->color == Color::WHITE) {
        moves = get_white_pawn_attacks(pawn, w_bb, b_bb);

        // enpassant
        // promotions.
        return BitboardHelper::remove_friendly_pieces(moves, w_bb);
    } else {

        moves = get_black_pawn_attacks(pawn , w_bb, b_bb);

        return BitboardHelper::remove_friendly_pieces(moves, b_bb);
    }
};

uint64_t Pawn::get_white_pawn_attacks(uint64_t pawn, uint64_t w_bb, uint64_t b_bb) {
    
    /* White pawns as of right now will always march in the northern direction. */

    uint64_t moves = 0ULL;
    this->captures = 0ULL;

    uint64_t white_pawn_start_rank = BitboardHelper::rank_masks[1];

    if (b_bb & (pawn << 9)) this->captures |= ((pawn & ~BitboardHelper::file_masks[7])<< 9);
    if (b_bb & (pawn << 7)) this->captures |= ((pawn & ~BitboardHelper::file_masks[0])<< 7);

    if (w_bb & (pawn << 8) | b_bb & (pawn << 8)) return moves;
    moves |= (pawn << 8);

    // Or doesn't seem right but works so eh.
    if (!(pawn & (white_pawn_start_rank)) | (b_bb & (pawn << 16))) return moves;
    moves |= (pawn << 16);

    return moves;
}

uint64_t Pawn::get_black_pawn_attacks(uint64_t pawn, uint64_t w_bb, uint64_t b_bb) {
    
    /* Black pawns as of right now will always march in the southern direction. */

    uint64_t moves = 0ULL;
    this->captures = 0ULL; // Reset so previous highlights not rendered.

    uint64_t black_pawn_start_rank = BitboardHelper::rank_masks[6];

    // Find the captures, mask out ones that overlap to next file.
    if (w_bb & (pawn >> 9)) this->captures |= (pawn & ~BitboardHelper::file_masks[0]) >> 9;
    if (w_bb & (pawn >> 7)) this->captures |= (pawn & ~BitboardHelper::file_masks[7]) >> 7;
    
    if (w_bb & (pawn >> 8) | b_bb & (pawn >> 8)) return moves;
    moves |= (pawn >> 8);

    // Or doesn't seem right but works so eh.
    if (!(pawn & (black_pawn_start_rank)) | (w_bb & (pawn >> 16))) return moves;
    moves |= (pawn >> 16);

    return moves;
}

/* KNIGHT */

uint64_t Knight::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {
    
    uint64_t knight = 1ULL << this->bit;
    uint64_t moves = 0ULL;

    // gunna be interesting when we need to flip the board.

    moves |= (knight & BitboardHelper::NOT_AB_FILE) << 6;
    moves |= (knight & BitboardHelper::NOT_A_FILE)  << 15; 
    moves |= (knight & BitboardHelper::NOT_H_FILE)  << 17;
    moves |= (knight & BitboardHelper::NOT_GH_FILE) << 10;
    
    moves |= (knight & BitboardHelper::NOT_GH_FILE) >> 6;   
    moves |= (knight & BitboardHelper::NOT_H_FILE)  >> 15;  
    moves |= (knight & BitboardHelper::NOT_A_FILE)  >> 17; 
    moves |= (knight & BitboardHelper::NOT_AB_FILE) >> 10; 

    uint64_t enemy = (this->color == Color::WHITE) ? b_bb : w_bb; 
    this->captures = (moves & enemy);

    return BitboardHelper::remove_friendly_pieces(moves, this->color == Color::WHITE ? w_bb : b_bb);
}

/* BISHOP */

uint64_t Bishop::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {
    uint64_t rook = (1ULL << this->bit);
    uint64_t moves = 0ULL;

    uint64_t north_west_moves = get_north_west_attacks(rook, w_bb, b_bb);
    uint64_t north_east_moves = get_north_east_attacks(rook, w_bb, b_bb);

    uint64_t south_west_moves = get_south_west_attacks(rook, w_bb, b_bb);
    uint64_t south_east_moves = get_south_east_attacks(rook, w_bb, b_bb);

    moves |= north_west_moves;
    moves |= north_east_moves;
    moves |= south_west_moves;
    moves |= south_east_moves;

    uint64_t enemy = (this->color == Color::WHITE) ? b_bb : w_bb; 
    this->captures = (moves & enemy);

    return BitboardHelper::remove_friendly_pieces(moves, (this->color == Color::WHITE) ? w_bb : b_bb);
};

uint64_t Piece::get_north_west_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_west_moves = 0ULL;
    const uint8_t north_west_offset = 9;

    uint8_t piece_file;
    uint8_t attack_file;
        
    piece_file = GRID_SZ - ((BitboardHelper::get_first_bit(piece)) % GRID_SZ) - 1;

    for (uint8_t i = 0; i < GRID_SZ; i++) {

        // this will be until we hit the end of the board, always 7 if no pieces in the way.
        attack_file = piece_file - i;

        if (w_bb & north_west_moves) break;
        if (b_bb & north_west_moves) break;
        if (!(piece << (north_west_offset*i))) break;

         // Stop generating attacks if we get to the far left file.
        if (attack_file == 0) break;

        north_west_moves |= (piece << (north_west_offset + (north_west_offset*i)));
    }
    return north_west_moves;
}

uint64_t Piece::get_north_east_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_east_moves = 0ULL;
    const uint8_t north_east_offset = 7;

    uint8_t piece_file;
    uint8_t attack_file;
    
    piece_file = GRID_SZ - ((BitboardHelper::get_first_bit(piece)) % GRID_SZ) - 1;

    for (uint8_t i = 0; i < GRID_SZ; i++) {

        attack_file = piece_file + i;

        if (w_bb & north_east_moves) break;
        if (b_bb & north_east_moves) break;
        if (!(piece << (north_east_offset*i))) break;

         // Stop generating attacks if we get to the far right file.
        if (attack_file == GRID_SZ - 1) break;

        north_east_moves |= (piece << (north_east_offset + (north_east_offset*i)));
    }
    return north_east_moves;
}

uint64_t Piece::get_south_west_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_west_moves = 0ULL;
    const uint8_t north_west_offset = 7;

    uint8_t piece_file;
    uint8_t attack_file;

    // Perhaps a bitboard function.
    piece_file = GRID_SZ - ((BitboardHelper::get_first_bit(piece)) % GRID_SZ) - 1;

    for (uint8_t i = 0; i < GRID_SZ; i++) {

        // this will be until we hit the end of the board, always 7 if no pieces in the way.
        attack_file = piece_file - i;

        if (w_bb & north_west_moves) break;
        if (b_bb & north_west_moves) break;
        if (!(piece >> (north_west_offset*i))) break;

         // Stop generating attacks if we get to the far left file.
        if (attack_file == 0) break;

        north_west_moves |= (piece >> (north_west_offset + (north_west_offset*i)));
    }
    return north_west_moves;
}

uint64_t Piece::get_south_east_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_east_moves = 0ULL;
    const uint8_t north_east_offset = 9;

    uint8_t piece_file;
    uint8_t attack_file;
    
    piece_file = GRID_SZ - ((BitboardHelper::get_first_bit(piece)) % GRID_SZ) - 1;

    for (uint8_t i = 0; i < GRID_SZ; i++) {

        attack_file = piece_file + i;

        if (w_bb & north_east_moves) break;
        if (b_bb & north_east_moves) break;
        if (!(piece >> (north_east_offset*i))) break;

         // Stop generating attacks if we get to the far right file.
        if (attack_file == GRID_SZ - 1) break;

        north_east_moves |= (piece >> (north_east_offset + (north_east_offset*i)));
    }
    return north_east_moves;
}

/* ROOK */

uint64_t Rook::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {

    uint64_t rook = (1ULL << this->bit);
    uint64_t moves = 0ULL;

    uint64_t north_moves = get_north_attacks(rook, w_bb, b_bb);
    uint64_t south_moves = get_south_attacks(rook, w_bb, b_bb);
    uint64_t west_moves = get_west_attacks(rook, w_bb, b_bb);
    uint64_t east_moves = get_east_attacks(rook, w_bb, b_bb);

    moves |= north_moves;
    moves |= south_moves;
    moves |= west_moves;
    moves |= east_moves;

    uint64_t enemy = (this->color == Color::WHITE) ? b_bb : w_bb; 
    this->moves =  (moves & enemy);

    return BitboardHelper::remove_friendly_pieces(moves, (this->color == Color::WHITE ? w_bb : b_bb));
};

uint64_t Piece::get_north_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_moves = 0ULL;
    uint8_t north_offset = 8;

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & north_moves) break;
        if (b_bb & north_moves) break;
        if (!(piece << (north_offset*i))) break;

        north_moves |= (piece << (north_offset + (north_offset*i)));

    }
    return north_moves;
}

uint64_t Piece::get_south_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t south_moves = 0ULL;
    uint8_t south_offset = 8;

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & south_moves) break;
        if (b_bb & south_moves) break;
        if (!(piece >> (south_offset*i))) break;

        south_moves |= (piece >> (south_offset + (south_offset*i)));

    }
    return south_moves;
}

uint64_t Piece::get_west_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t west_moves = 0ULL;
    uint8_t west_offset = 1;
    
    // Gets the index into rank_masks of the rank above the piece so we can mask it when shifting <<.
    uint8_t piece_bit = BitboardHelper::get_first_bit(piece);
    uint8_t mask_index = (piece_bit / GRID_SZ) + 1;

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & west_moves) break;
        if (b_bb & west_moves) break;
        if (!(piece << (west_offset*i))) break; // Can we continue shifting? If not then off the board.

        west_moves |= (piece << (west_offset + (west_offset*i)));
    }
    
    return (west_moves & ~(BitboardHelper::rank_masks[mask_index]));
}

uint64_t Piece::get_east_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t east_moves = 0ULL;
    uint8_t east_offset = 1;
    
    // Gets the index into rank_masks of the rank above the piece so we can mask it when shifting <<.
    uint8_t piece_bit = BitboardHelper::get_first_bit(piece);
    uint8_t mask_index = (piece_bit / GRID_SZ) - 1; // Minus for east.

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & east_moves) break;
        if (b_bb & east_moves) break;
        if (!(piece >> (east_offset*i))) break; // Can we continue shifting? If not then off the board.

        east_moves |= (piece >> (east_offset + (east_offset*i)));
    }
    
    return (east_moves & ~(BitboardHelper::rank_masks[mask_index]));
}

/* QUEEN */

uint64_t Queen::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {

    uint64_t queen = (1ULL << this->bit);
    uint64_t moves = 0ULL;

    uint64_t north_moves = get_north_attacks(queen, w_bb, b_bb);
    uint64_t south_moves = get_south_attacks(queen, w_bb, b_bb);
    uint64_t west_moves = get_west_attacks(queen, w_bb, b_bb);
    uint64_t east_moves = get_east_attacks(queen, w_bb, b_bb);

    uint64_t north_west_moves = get_north_west_attacks(queen, w_bb, b_bb);
    uint64_t north_east_moves = get_north_east_attacks(queen, w_bb, b_bb);
    uint64_t south_west_moves = get_south_west_attacks(queen, w_bb, b_bb);
    uint64_t south_east_moves = get_south_east_attacks(queen, w_bb, b_bb);

    moves |= north_moves;
    moves |= south_moves;
    moves |= west_moves;
    moves |= east_moves;

    moves |= north_west_moves;
    moves |= north_east_moves;
    moves |= south_west_moves;
    moves |= south_east_moves;

    uint64_t enemy_occupancy = (this->color == Color::WHITE) ? b_bb : w_bb; 
    this->captures = (moves & enemy_occupancy);
    
    return BitboardHelper::remove_friendly_pieces(moves, (this->color == Color::WHITE ? w_bb : b_bb));
};

/* KING */

uint64_t King::get_legal_moves(uint64_t w_bb, uint64_t b_bb) {};