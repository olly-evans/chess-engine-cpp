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

    std::string piece_id;

    std::string resolve_texture_path();

    /* NOT MASKS */ 

    const uint64_t NOT_A_FILE  = 0xFEFEFEFEFEFEFEFEULL;
    const uint64_t NOT_AB_FILE = 0xFCFCFCFCFCFCFCFCULL;
    const uint64_t NOT_H_FILE  = 0x7F7F7F7F7F7F7F7FULL;
    const uint64_t NOT_GH_FILE = 0x3F3F3F3F3F3F3F3FULL;

public:
    Piece(std::string id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz);

    sf::Vector2f pos;

    uint64_t attacks = 0ULL;
    uint64_t bit = 0ULL;
    
    virtual std::string get_texture_path();

    void draw(sf::RenderWindow& window);

    virtual uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) = 0;
    void highlight_legal_moves(uint64_t attacks, std::vector<sf::RectangleShape>& squareuint64_t);
};

class Pawn : public Piece {
public:
    Pawn(std::string id, Color col, sf::RenderWindow& w, uint8_t bitboard, int b_squ_sz) : 
        Piece("P", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
    uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) override;

};

class Knight : public Piece {
public:
    const int offsets[8][2] = {
        {-2, -1}, {-2, 1}, {2, -1}, {2, 1},
        {-1, -2}, {1, -2}, {-1, 2}, {1, 2}
    };

    Knight(std::string id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz) : 
        Piece("N", col, w, b, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
    
    uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) override;

};

class Bishop : public Piece {
public:
    Bishop(std::string id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz) : 
        Piece("B", col, w, b, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
    
    uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) override;

};

class Rook : public Piece {
public:
    Rook(std::string id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz) : 
        Piece("R", col, w, b, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }

    uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) override;
};

class Queen : public Piece {
public:
    Queen(std::string id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz) : 
        Piece("Q", col, w, b, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }

    uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) override;

};

class King : public Piece {
public:
    King(std::string id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz) : 
        Piece("K", col, w, b, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }

    uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) override;

};