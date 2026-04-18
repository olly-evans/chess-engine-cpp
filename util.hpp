#pragma once

#include <SFML/Graphics.hpp>

constexpr unsigned int GRID_SZ = 8;
constexpr unsigned int GRID_NUM_SQUARES = GRID_SZ * GRID_SZ;

static sf::Vector2f index_to_2d(int i) {
        return sf::Vector2f(i % GRID_SZ, i / GRID_SZ);
}