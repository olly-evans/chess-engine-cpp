#pragma once

#include <cstdint>
#include <SFML/Window.hpp>

constexpr unsigned int GRID_SZ = 8;
constexpr unsigned int GRID_NUM_SQUARES = GRID_SZ * GRID_SZ;

enum Bitboards { W_PAWNS, W_KNIGHTS, W_BISHOPS, W_ROOKS, W_QUEEN, W_KING,
                 B_PAWNS, B_KNIGHTS, B_BISHOPS, B_ROOKS, B_QUEEN, B_KING 
};

/* INDEXING */

static int pos2d_to_index(sf::Vector2f pos) {
        return ((pos.y * GRID_SZ) + pos.x);
}

/* POSITIONS */

inline bool is_vecs_equal(sf::Vector2f vec1, sf::Vector2f vec2) {
        return vec1.x == vec2.x && vec1.y == vec2.y;
}