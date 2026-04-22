#pragma once

#include <cstdint>

class BitboardHelper {
public:
    // uint64_t piece_type_bitboards;

    // static uint64_t white_occupancy;
    // static uint64_t white_occupancy;

    static int square_to_bit(int square);

    // manipulation
    static uint64_t set_bit(uint64_t b, int square);
    static uint64_t clear_bit(uint64_t b, int square);
    static bool get_bit(uint64_t b, int square);

    /* CHECKS */
    static bool has_friendly_piece(uint64_t friendly_bb, int square);
    static uint64_t remove_friendly_pieces(uint64_t attacks, uint64_t friendly);

    
    /* MOVE GENERATION */

    // static uint64_t rank_mask(int square);
    // static uint64_t file_mask(int square);
    // static uint64_t diagonal_mask(int square);

};