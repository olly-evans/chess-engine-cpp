#include "bitboardhelper.hpp"
#include "util.hpp"

#include <cmath>

/* MANIPULATION */

int BBHelper::square_to_bit(int square) {
    return GRID_NUM_SQUARES - square - 1;
}

int BBHelper::bit_to_square(int bit) {
    return GRID_NUM_SQUARES - bit - 1;
}

void BBHelper::set_bit_by_ref(uint64_t& b, int bit) {
    b |= (1ULL << bit);
}

uint64_t BBHelper::set_bit(uint64_t b, int bit) {
    return (b) |= (1ULL << bit);
}

uint64_t BBHelper::clear_bit(uint64_t b, int bit) {
    return (b) &= ~(1ULL << bit);
}

bool BBHelper::get_bit(uint64_t b, int bit) {
    return b & (1ULL << bit);
}

uint8_t BBHelper::get_first_bit(uint64_t b) {
    
    /* Return the first bit encountered in a bitboard. */

    for (uint8_t i = 0; i < GRID_NUM_SQUARES; i++) {
        if (b & (1ULL << i)) return i;
    }
    
    return -1;
}

uint8_t BBHelper::get_piece_file(uint64_t piece) {
    return GRID_SZ - ((BBHelper::get_first_bit(piece)) % GRID_SZ) - 1;
}

/* CHECKS */

bool BBHelper::has_friendly_piece(uint64_t friendly_bb, int bit) {
    return friendly_bb & (1ULL << bit);
}

uint64_t BBHelper::remove_friendly_pieces(uint64_t attacks, uint64_t friendly) {
    return attacks &= ~friendly;
}

uint64_t BBHelper::remove_enemy_pieces(uint64_t moves, uint64_t enemy) {
    return moves &= ~enemy;
}

uint8_t BBHelper::get_total_active_bits(uint64_t b) {

        uint8_t count = 0;
        for (int i = 0; i < GRID_NUM_SQUARES; i++) {
            if (b & (1ULL << i)) count++;
        }
        return count;
    }
    
/* PIECE MOVEMENT */

/* MAKE ALL THIS PART OF PIECE */

