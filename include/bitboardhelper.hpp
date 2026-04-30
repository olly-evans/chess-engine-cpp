#pragma once

#include <cstdint>
#include <vector>

class BitboardHelper {
public:
    // File masks.
    static const uint64_t NOT_A_FILE  = 0xFEFEFEFEFEFEFEFEULL;
    static const uint64_t NOT_AB_FILE = 0xFCFCFCFCFCFCFCFCULL;
    static const uint64_t NOT_H_FILE  = 0x7F7F7F7F7F7F7F7FULL;
    static const uint64_t NOT_GH_FILE = 0x3F3F3F3F3F3F3F3FULL;

    // Rank masks.
    inline static std::vector<uint64_t> rank_masks = {
        0x00000000000000FF, // bottom, 0, 1 rank
        0x000000000000FF00,
        0x0000000000FF0000,
        0x00000000FF000000,
        0x000000FF00000000,
        0x0000FF0000000000,
        0x00FF000000000000,
        0xFF00000000000000 // top, 7, 8th rank. white pov. ik im sorry.

    };

    inline static std::vector<uint64_t> file_masks = {
        0x0101010101010101ULL, // File A
        0x0202020202020202ULL, // File B
        0x0404040404040404ULL, // File C
        0x0808080808080808ULL, // File D
        0x1010101010101010ULL, // File E
        0x2020202020202020ULL, // File F
        0x4040404040404040ULL, // File G
        0x8080808080808080ULL  // File H
    };
 

    static int square_to_bit(int square);
    static int bit_to_square(int bit);

    // manipulation
    static void set_bit_by_ref(uint64_t& b, int bit);
    static uint64_t set_bit(uint64_t b, int bit);
    static uint64_t clear_bit(uint64_t b, int bit);
    static bool get_bit(uint64_t b, int square);
    static uint8_t get_first_bit(uint64_t b);


    /* CHECKS */
    static bool has_friendly_piece(uint64_t friendly_bb, int bit);
    static uint64_t remove_friendly_pieces(uint64_t attacks, uint64_t friendly);
};