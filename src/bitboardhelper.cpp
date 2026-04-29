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

// Maybe this will be useful, condition for north square valid attack.

// bool BitboardHelper::north_square_valid(uint64_t piece, uint64_t north_attacks, uint64_t w_bb, uint64_t b_bb) {
//     return (!(w_bb & north_attacks) | !(b_bb & north_attacks)) && (piece << (8*i);
// }

uint64_t BitboardHelper::get_viable_north_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_attacks = 0ULL;
    uint8_t north_offset = 8;

    // uint8_t i = 0;
    // while ((!(w_bb & north_attacks) || !(b_bb & north_attacks)) && (piece << (north_offset*i))) {
    //     north_attacks |= (piece << (north_offset + (north_offset*i)));
    //     i++;
    // }

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & north_attacks) return north_attacks;
        if (b_bb & north_attacks) return north_attacks;
        if (!(piece << (north_offset*i))) return north_attacks;

        north_attacks |= (piece << (north_offset + (north_offset*i)));

    }
    return north_attacks;
}

uint64_t BitboardHelper::get_viable_south_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t south_attacks = 0ULL;
    uint8_t south_offset = 8;

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & south_attacks) return south_attacks;
        if (b_bb & south_attacks) return south_attacks;
        if (!(piece >> (south_offset*i))) return south_attacks;

        south_attacks |= (piece >> (south_offset + (south_offset*i)));

    }

    // not 100% on this.
    return south_attacks;
}

uint64_t BitboardHelper::get_viable_west_attacks(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {
    uint64_t west_attacks = 0ULL;
    uint8_t west_offset = 1;

    // need the bit. convert bit to rank. rank + or -1 depending on color i think


    uint64_t rank_0 = 0xFF00000000000000;
    uint64_t rank_1 = 0x00FF000000000000;
    uint64_t rank_2 = 0x0000FF0000000000;
    uint64_t rank_3 = 0x000000FF00000000;
    uint64_t rank_4 = 0x00000000FF000000;
    uint64_t rank_5 = 0x0000000000FF0000;
    uint64_t rank_6 = 0x000000000000FF00;
    uint64_t rank_7 = 0x00000000000000FF;
    // perhaps we can use i here actually???
    // to get the correct rank, dont think so acc.

    // divide bit by 8 with integer div gives the rank above hehe. <---------------

    // these mask names mean nothing ill be real.
    for (uint8_t i = 0; i < GRID_SZ; i++) {
        west_attacks |= (piece << (west_offset + (west_offset*i)));

        // think this needs to dynamically be the rank above the rook pos.
        if (w_bb & west_attacks) return (west_attacks & ~rank_0);
        // diff mask for diff color i think, maybe not actually.

        if (b_bb & west_attacks) return (west_attacks & ~rank_0);
        if (!(piece << (west_offset*i))) return (west_attacks & ~rank_0);

        // these masks are fucked from the other person pov, might not be actually. east will handle this for black and vice versa i believe.


    }
    return (west_attacks & ~rank_0);
}
