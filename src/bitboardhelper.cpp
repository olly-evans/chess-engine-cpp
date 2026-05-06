#include "bitboardhelper.hpp"
#include "util.hpp"

#include <cmath>

const std::string BBHelper::bit_to_square_name[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1" 
};

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

