#include "bitboardhelper.hpp"
#include "util.hpp"

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

    uint8_t i = 0;
    while ((!(w_bb & north_attacks) || !(b_bb & north_attacks)) && (piece << (north_offset*i))) {
        north_attacks |= (piece << (north_offset + (north_offset*i)));
        i++;
    }
    return north_attacks;
}