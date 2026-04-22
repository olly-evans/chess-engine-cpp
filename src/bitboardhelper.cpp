#include "bitboardhelper.hpp"
#include "util.hpp"

/* MANIPULATION */

int BitboardHelper::square_to_bit(int square) {
    return GRID_NUM_SQUARES - square - 1;
}

uint64_t BitboardHelper::set_bit(uint64_t b, int square) {

    int bit = BitboardHelper::square_to_bit(square);
    return (b) |= (1ULL << bit);
}

uint64_t BitboardHelper::clear_bit(uint64_t b, int square) {

    int bit = BitboardHelper::square_to_bit(square);
    return (b) &= ~(1ULL << bit);
}

bool BitboardHelper::get_bit(uint64_t b, int square) {
    int bit = BitboardHelper::square_to_bit(square);
    return b & (1ULL << bit);
}

/* CHECKS */

bool BitboardHelper::has_friendly_piece(uint64_t friendly_bb, int square) {
    int bit = BitboardHelper::square_to_bit(square);
    return friendly_bb & (1ULL << bit);
}
