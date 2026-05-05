#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>

#include <vector>
#include <string>
#include <filesystem>
#include <cstdint>

#include "util.hpp"

enum class Color {BLACK, WHITE};

class Piece {

protected:
    uint8_t board_square_size;

    sf::Texture texture;
    sf::Sprite sprite;
    
    sf::RenderWindow& window;


    std::string resolve_texture_path();


public:
    Piece(char id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz);

    Color color;

    char piece_id;

    uint64_t moves = 0ULL;
    uint64_t captures = 0ULL;
    uint64_t bit = 0ULL;
    
    virtual std::string get_texture_path();

    void draw(sf::RenderWindow& window);

    // All instances return the moves and assign this->captures.
    virtual uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) = 0;

    uint64_t get_north_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_south_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_west_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_east_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);

    uint64_t get_north_west_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_north_east_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_south_west_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_south_east_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);

};

class Pawn : public Piece {
public:
    Pawn(char id, Color col, sf::RenderWindow& w, uint8_t bitboard, int b_squ_sz) : 
        Piece(id, col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }

    uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) override;

    uint64_t get_white_pawn_moves(uint64_t pawn, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_black_pawn_moves(uint64_t pawn, uint64_t w_bb, uint64_t b_bb);

    uint64_t get_enpassant(uint64_t w_bb, uint64_t b_bb);

};

class Knight : public Piece {
public:
    const int offsets[8][2] = {
        {-2, -1}, {-2, 1}, {2, -1}, {2, 1},
        {-1, -2}, {1, -2}, {-1, 2}, {1, 2}
    };

    Knight(char id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz) : 
        Piece(id, col, w, b, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
    
    uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) override;

};

class Bishop : public Piece {
public:
    Bishop(char id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz) : 
        Piece(id, col, w, b, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
    
    uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) override;

};

class Rook : public Piece {
public:
    Rook(char id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz) : 
        Piece(id, col, w, b, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }

    uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) override;

};

class Queen : public Piece {
public:
    Queen(char id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz) : 
        Piece(id, col, w, b, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }

    uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) override;

};

class King : public Piece {
public:
    King(char id, Color col, sf::RenderWindow& w, uint8_t b, int b_squ_sz) : 
        Piece(id, col, w, b, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }

    uint64_t get_legal_moves(uint64_t w_bb, uint64_t b_bb) override;

};