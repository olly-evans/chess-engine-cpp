#pragma once

#include <cstdint>
#include <SFML/Window.hpp>

constexpr unsigned int GRID_SZ = 8;
constexpr unsigned int GRID_NUM_SQUARES = GRID_SZ * GRID_SZ;

enum PieceID {

    // Represents starting piece index.
    BR_A8 = 0, BN_B8, BB_C8, BQ_D8, BK_E8, BB_F8, BN_G8, BR_H8,
    BP_A7, BP_B7, BP_C7, BP_D7, BP_E7, BP_F7, BP_G7, BP_H7,

    WP_A2 = 48, WP_B2, WP_C2, WP_D2, WP_E2, WP_F2, WP_G2, WP_H2,  
    WR_A1, WN_B1, WB_C1, WQ_D1, WK_E1, WB_F1, WN_G1, WR_H1,
      
};

enum Bitboards { W_PAWNS, W_KNIGHTS, W_BISHOPS, W_ROOKS, W_QUEEN, W_KING,
                 B_PAWNS, B_KNIGHTS, B_BISHOPS, B_ROOKS, B_QUEEN, B_KING 
};


/* BITBOARD HELPERS */

inline bool is_bit_set(uint64_t bitboard, int index) {
        return (bitboard & (1ULL << index));
}

/* INDEXING */

static sf::Vector2f index_to_2d(int i) {
        return sf::Vector2f(i % GRID_SZ, i / GRID_SZ);
}

static int pos2d_to_index(sf::Vector2f pos) {
        return ((pos.y * GRID_SZ) + pos.x);
}

/* POSITIONS */

inline bool is_vecs_equal(sf::Vector2f vec1, sf::Vector2f vec2) {
        return vec1.x == vec2.x && vec1.y == vec2.y;
}