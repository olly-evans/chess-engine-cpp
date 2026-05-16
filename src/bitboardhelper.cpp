#include "bitboardhelper.hpp"
#include "util.hpp"
#include "sfml_renderer.hpp"

#include <cmath>

const std::string BBHelper::bit_to_square_name[] = {
    "h1", "g1", "f1", "e1", "d1", "c1", "b1", "a1",
    "h2", "g2", "f2", "e2", "d2", "c2", "b2", "a2",
    "h3", "g3", "f3", "e3", "d3", "c3", "b3", "a3",
    "h4", "g4", "f4", "e4", "d4", "c4", "b4", "a4",
    "h5", "g5", "f5", "e5", "d5", "c5", "b5", "a5",
    "h6", "g6", "f6", "e6", "d6", "c6", "b6", "a6",
    "h7", "g7", "f7", "e7", "d7", "c7", "b7", "a7",
    "h8", "g8", "f8", "e8", "d8", "c8", "b8", "a8"
};

std::unordered_map<std::string, uint8_t> BBHelper::name_to_index;

void BBHelper::init_name_to_bit() {
    // Fill the map using your existing bit_to_square_name array
    for (uint8_t i = 0; i < 64; ++i) {
        name_to_index[bit_to_square_name[i]] = i;
    }
}

uint8_t BBHelper::square_name_to_bit(const std::string& name) {
    auto it = name_to_index.find(name);
    return (it != name_to_index.end()) ? it->second : -1;
}

/* MANIPULATION */

int BBHelper::square_to_bit(int square) {
    return GRID_NUM_SQUARES - square - 1;
}

uint8_t BBHelper::bit_to_square(uint8_t bit) {
    return GRID_NUM_SQUARES - bit - 1;
}

void BBHelper::set_bit_by_ref(uint64_t& b, int bit) {
    b |= (1ULL << bit);
}

void BBHelper::clear_bit_by_ref(uint64_t& b, int bit) {
    b &= ~(1ULL << bit);
}

uint64_t BBHelper::set_bit(uint64_t b, int bit) {
    return (b) |= (1ULL << bit);
}

uint64_t BBHelper::clear_bit(uint64_t b, int bit) {
    return (b) &= ~(1ULL << bit);
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

/* GETS */

bool BBHelper::get_bit(uint64_t b, int bit) {
    return b & (1ULL << bit);
}

uint8_t BBHelper::get_first_bit(uint64_t b) {
    
    /* Return the first bit encountered in a bitboard from H1. */

    for (uint8_t i = 0; i < GRID_NUM_SQUARES; i++) {
        if (b & (1ULL << i)) 
            return i;
    }
    
    return -1;
}

uint8_t BBHelper::get_piece_file(uint64_t piece) {
    return GRID_SZ - ((BBHelper::get_first_bit(piece)) % GRID_SZ) - 1;
}

uint8_t BBHelper::get_set_bits(uint64_t b) {

    uint8_t c;
    for (c = 0; b; c++) {
        b &= b - 1; // clear the lsb set.
    }
    return c;
}

std::vector<uint8_t> BBHelper::get_bit_vector(uint64_t bitboard) {

    // slow and awful.
    // __builtin_clz
    std::vector<uint8_t> bits;
    for (uint8_t i = 0; i < GRID_NUM_SQUARES; i++) {
        if (BBHelper::get_bit(bitboard, i))
            bits.push_back(i);
    }
    return bits;
}