#include "pieces.hpp"
#include "board.hpp"
#include "bitboardhelper.hpp"
#include "movelogger.hpp"
#include "sfml_app.hpp"

#include <iostream>
#include <filesystem>
#include <cstdint>
#include <string>

Piece::Piece(char id, uint8_t b) : id(id), bit(b) 
{
    
    is_white = (isupper(this->id));

    file = BBHelper::get_piece_file(this->bit);

    // need to assign it a texture from the cache.
    // texture_cache[id] = std::move(texture);

}

void Piece::set_bit(uint8_t bit) {
    this->bit = bit;
    this->file = BBHelper::get_piece_file(this->bit);
    // this->rank = BBHelper::get_piece_rank(this->bit);
    // this->has_moved = true;
}

uint8_t Piece::get_bit() {
    return this->bit;
}

void Piece::strip_pseudo_legal_attacks(Board& board) {

    /* 
    *
    *  Strips piece members this->moves and this->captures of attacks that would result 
    *  in a friendly king check by simulating all of the moves. Yes, slow but works
    *  for now.
    * 
    */

    uint64_t attacks = this->moves | this->captures;

    // Lets try and do this better.
    std::vector<uint8_t> move_bits = BBHelper::get_bit_vector(attacks);

    // perhaps use move struct. not essential but is semantic
    for (uint8_t move_bit : move_bits) {

        uint64_t enemy_captures = board.get_simulated_enemy_captures(this, this->bit, move_bit, move_bit);

        uint64_t friendly_king;

        // Make a fake king if we have a king selected, else use the real one.
        if (toupper(this->id) == 'K') {
            friendly_king = (1ULL << move_bit);
        } else {
            friendly_king = (this->is_white) ? board.bitboards[FenParser::W_KING] : board.bitboards[FenParser::B_KING];
        }        

        bool in_check = friendly_king & enemy_captures;

        // Strip from moves or captures any attack that would result in friendly king being in check.
        if (in_check && BBHelper::get_bit(this->moves, move_bit))
            this->moves = BBHelper::clear_bit(this->moves, move_bit);
        
        if (in_check && BBHelper::get_bit(this->captures, move_bit))
            this->captures = BBHelper::clear_bit(this->captures, move_bit);

    }
    // I don't like this, but at least its modular and in the general stripping function.
    strip_pseudo_legal_special_moves(board);
}

uint64_t Piece::get_north_west_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_west_moves = 0ULL;
    const uint8_t north_west_offset = 9;

    uint8_t move_file;
        
    for (uint8_t i = 0; i < GRID_SZ; i++) {

        // this will be until we hit the end of the board, always 7 if no pieces in the way.
        move_file = this->file - i;

        if (w_bb & north_west_moves) break;
        if (b_bb & north_west_moves) break;
        if (!(piece << (north_west_offset*i))) break;

         // Stop generating attacks if we get to the far left file.
        if (move_file == 0) break;

        north_west_moves |= (piece << (north_west_offset + (north_west_offset*i)));
    }
    return north_west_moves;
}

uint64_t Piece::get_north_east_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_east_moves = 0ULL;
    const uint8_t north_east_offset = 7;

    uint8_t move_file;
    
    for (uint8_t i = 0; i < GRID_SZ; i++) {

        move_file = this->file + i;

        if (w_bb & north_east_moves) break;
        if (b_bb & north_east_moves) break;
        if (!(piece << (north_east_offset*i))) break;

         // Stop generating attacks if we get to the far right file.
        if (move_file == GRID_SZ - 1) break;

        north_east_moves |= (piece << (north_east_offset + (north_east_offset*i)));
    }
    return north_east_moves;
}

uint64_t Piece::get_south_west_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_west_moves = 0ULL;
    const uint8_t north_west_offset = 7;

    uint8_t move_file;

    for (uint8_t i = 0; i < GRID_SZ; i++) {

        // this will be until we hit the end of the board, always 7 if no pieces in the way.
        move_file = this->file - i;

        if (w_bb & north_west_moves) break;
        if (b_bb & north_west_moves) break;
        if (!(piece >> (north_west_offset*i))) break;

         // Stop generating attacks if we get to the far left file.
        if (move_file == 0) break;

        north_west_moves |= (piece >> (north_west_offset + (north_west_offset*i)));
    }
    return north_west_moves;
}

uint64_t Piece::get_south_east_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_east_moves = 0ULL;
    const uint8_t north_east_offset = 9;

    uint8_t move_file;
    
    for (uint8_t i = 0; i < GRID_SZ; i++) {

        move_file = this->file + i;

        if (w_bb & north_east_moves) break;
        if (b_bb & north_east_moves) break;
        if (!(piece >> (north_east_offset*i))) break;

         // Stop generating attacks if we get to the far right file.
        if (move_file == GRID_SZ - 1) break;

        north_east_moves |= (piece >> (north_east_offset + (north_east_offset*i)));
    }
    return north_east_moves;
}



/* ROOK */



uint64_t Piece::get_north_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

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

uint64_t Piece::get_south_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

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

uint64_t Piece::get_west_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t west_moves = 0ULL;
    uint8_t west_offset = 1;
    
    // Gets the index into rank_masks of the rank above the piece so we can mask it when shifting <<.
    uint8_t piece_bit = BBHelper::get_first_bit(piece);
    uint8_t mask_index = (piece_bit / GRID_SZ) + 1;

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & west_moves) break;
        if (b_bb & west_moves) break;
        if (!(piece << (west_offset*i))) break; // Can we continue shifting? If not then off the board.

        west_moves |= (piece << (west_offset + (west_offset*i)));
    }
    
    return (west_moves & ~(BBHelper::rank_masks[mask_index]));
}

uint64_t Piece::get_east_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t east_moves = 0ULL;
    uint8_t east_offset = 1;
    
    // Gets the index into rank_masks of the rank above the piece so we can mask it when shifting <<.
    uint8_t piece_bit = BBHelper::get_first_bit(piece);
    uint8_t mask_index = (piece_bit / GRID_SZ) - 1; // Minus for east.

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & east_moves) break;
        if (b_bb & east_moves) break;
        if (!(piece >> (east_offset*i))) break; // Can we continue shifting? If not then off the board.

        east_moves |= (piece >> (east_offset + (east_offset*i)));
    }

    return (east_moves & ~(BBHelper::rank_masks[mask_index]));
}