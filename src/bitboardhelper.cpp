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

uint64_t BitboardHelper::get_viable_north_attacks(uint64_t piece, uint64_t white_occupancy, uint64_t black_occupancy) {

    uint64_t north_attacks = 0ULL;

    int i = 0;

    // condition wrong, goes off the board i think.
    while (!(white_occupancy & north_attacks) | !(black_occupancy & north_attacks)) {
        north_attacks |= (piece << (8 + (8*i)));
        i++;
    }
    return north_attacks;

}