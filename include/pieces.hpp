#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdint>
#include "util.hpp"

enum class Color {BLACK, WHITE};

class Piece {

protected:
    int board_square_size;

    sf::Texture texture;
    sf::Sprite sprite;

    Color color;

    sf::RenderWindow& window;

    std::vector<sf::Vector2f> piece_positions;
    // assert king/queen piece_pos.size() == 1.

    std::string piece_id;

    std::string resolve_texture_path();

public:
    uint64_t bitboard;

    Piece(std::string id, Color col, sf::RenderWindow& w, uint64_t bitb, int b_squ_sz);

    virtual std::string get_texture_path();

    void draw(sf::RenderWindow& window);

    void init_piece_positions_vector_from_bitboard(uint64_t bitboard, std::vector<sf::Vector2f> &piece_pos);

    void render_highlight(sf::Vector2f piece_position);
};

class Pawn : public Piece {
public:
    Pawn(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("P", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
};

class Knight : public Piece {
public:
    Knight(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("N", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
    
};

class Bishop : public Piece {
public:
    Bishop(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("B", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
};

class Rook : public Piece {
public:
    Rook(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("R", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
};

class Queen : public Piece {
public:
    Queen(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("Q", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
};

class King : public Piece {
public:
    King(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("K", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
};