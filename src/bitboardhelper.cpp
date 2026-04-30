#include "bitboardhelper.hpp"
#include "util.hpp"

#include <cmath>

/* MANIPULATION */

int BitboardHelper::square_to_bit(int square) {
    return GRID_NUM_SQUARES - square - 1;
}

int BitboardHelper::bit_to_square(int bit) {
    return GRID_NUM_SQUARES - bit - 1;
}

void BitboardHelper::set_bit_by_ref(uint64_t& b, int bit) {
    b |= (1ULL << bit);
}

uint64_t BitboardHelper::set_bit(uint64_t b, int bit) {
    return (b) |= (1ULL << bit);
}

uint64_t BitboardHelper::clear_bit(uint64_t b, int bit) {
    return (b) &= ~(1ULL << bit);
}

bool BitboardHelper::get_bit(uint64_t b, int bit) {
    return b & (1ULL << bit);
}

uint8_t BitboardHelper::get_first_bit(uint64_t b) {
    
    /* Return the first bit encountered in a bitboard. */

    for (uint8_t i = 0; i < GRID_NUM_SQUARES; i++) {
        if (b & (1ULL << i)) return i;
    }
}

/* CHECKS */

bool BitboardHelper::has_friendly_piece(uint64_t friendly_bb, int bit) {
    return friendly_bb & (1ULL << bit);
}

uint64_t BitboardHelper::remove_friendly_pieces(uint64_t attacks, uint64_t friendly) {
    return attacks &= ~friendly;
}

/* PIECE MOVEMENT */

// Maybe this will be useful, condition for north square valid attack.

// bool BitboardHelper::north_square_valid(uint64_t piece, uint64_t north_attacks, uint64_t w_bb, uint64_t b_bb) {
//     return (!(w_bb & north_attacks) | !(b_bb & north_attacks)) && (piece << (8*i);
// }

uint64_t BitboardHelper::get_viable_north_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_attacks = 0ULL;
    uint8_t north_offset = 8;

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & north_attacks) break;
        if (b_bb & north_attacks) break;
        if (!(piece << (north_offset*i))) break;

        north_attacks |= (piece << (north_offset + (north_offset*i)));

    }
    return north_attacks;
}

uint64_t BitboardHelper::get_viable_south_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t south_attacks = 0ULL;
    uint8_t south_offset = 8;

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & south_attacks) break;
        if (b_bb & south_attacks) break;
        if (!(piece >> (south_offset*i))) break;

        south_attacks |= (piece >> (south_offset + (south_offset*i)));

    }

    return south_attacks;
}

uint64_t BitboardHelper::get_viable_west_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t west_attacks = 0ULL;
    uint8_t west_offset = 1;
    
    // Gets the index into rank_masks of the rank above the piece so we can mask it when shifting <<.
    uint8_t piece_bit = BitboardHelper::get_first_bit(piece);
    float rank_to_mask = float(piece_bit) / float(GRID_SZ);
    uint8_t index = std::ceil(rank_to_mask);

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & west_attacks) break;
        if (b_bb & west_attacks) break;
        if (!(piece << (west_offset*i))) break;

        west_attacks |= (piece << (west_offset + (west_offset*i)));
    }
    
    return (west_attacks & ~(BitboardHelper::rank_masks[index]));
}
