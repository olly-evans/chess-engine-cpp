#pragma once

#include <cstdint>

class BitboardHelper {
public:

    static int square_to_bit(int square);
    static int bit_to_square(int bit);

    // manipulation
    static void set_bit_by_ref(uint64_t& b, int bit);
    static uint64_t set_bit(uint64_t b, int bit);
    static uint64_t clear_bit(uint64_t b, int bit);
    static bool get_bit(uint64_t b, int square);

    /* CHECKS */
    static bool has_friendly_piece(uint64_t friendly_bb, int square);
    static uint64_t remove_friendly_pieces(uint64_t attacks, uint64_t friendly);

    
    /* MOVE GENERATION */

    static uint64_t get_viable_north_attacks(uint64_t piece, uint64_t white_occupancy, uint64_t black_occupancy);
    static uint64_t get_viable_south_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb);


};