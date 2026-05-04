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

uint8_t BitboardHelper::get_piece_file(uint64_t piece) {
    return GRID_SZ - ((BitboardHelper::get_first_bit(piece)) % GRID_SZ) - 1;
}

/* CHECKS */

bool BitboardHelper::has_friendly_piece(uint64_t friendly_bb, int bit) {
    return friendly_bb & (1ULL << bit);
}

uint64_t BitboardHelper::remove_friendly_pieces(uint64_t attacks, uint64_t friendly) {
    return attacks &= ~friendly;
}

uint64_t BitboardHelper::remove_enemy_pieces(uint64_t moves, uint64_t enemy) {
    return moves &= ~enemy;
}

uint8_t BitboardHelper::get_total_active_bits(uint64_t b) {

        uint8_t count = 0;
        for (int i = 0; i < GRID_NUM_SQUARES; i++) {
            if (b & (1ULL << i)) count++;
        }
        return count;
    }
    
/* PIECE MOVEMENT */

/* MAKE ALL THIS PART OF PIECE */

