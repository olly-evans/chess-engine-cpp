#pragma once

#include <cstdint>

class BitboardHelper {
public:
    // File masks.
    static const uint64_t NOT_A_FILE  = 0xFEFEFEFEFEFEFEFEULL;
    static const uint64_t NOT_AB_FILE = 0xFCFCFCFCFCFCFCFCULL;
    static const uint64_t NOT_H_FILE  = 0x7F7F7F7F7F7F7F7FULL;
    static const uint64_t NOT_GH_FILE = 0x3F3F3F3F3F3F3F3FULL;

    // Rank masks.
    static const uint64_t rank_0 = 0xFF00000000000000;
    static const uint64_t rank_1 = 0x00FF000000000000;
    static const uint64_t rank_2 = 0x0000FF0000000000;
    static const uint64_t rank_3 = 0x000000FF00000000;
    static const uint64_t rank_4 = 0x00000000FF000000;
    static const uint64_t rank_5 = 0x0000000000FF0000;
    static const uint64_t rank_6 = 0x000000000000FF00;
    static const uint64_t rank_7 = 0x00000000000000FF;

    static int square_to_bit(int square);
    static int bit_to_square(int bit);

    // manipulation
    static void set_bit_by_ref(uint64_t& b, int bit);
    static uint64_t set_bit(uint64_t b, int bit);
    static uint64_t clear_bit(uint64_t b, int bit);
    static bool get_bit(uint64_t b, int square);
    static uint8_t get_first_bit(uint64_t b);


    /* CHECKS */
    static bool has_friendly_piece(uint64_t friendly_bb, int square);
    static uint64_t remove_friendly_pieces(uint64_t attacks, uint64_t friendly);

    
    /* MOVE GENERATION */

    static uint64_t get_viable_north_attacks(uint64_t piece, uint64_t white_occupancy, uint64_t black_occupancy);
    static uint64_t get_viable_south_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    static uint64_t get_viable_west_attacks(uint64_t rook, uint64_t w_bb, uint64_t b_bb);


};