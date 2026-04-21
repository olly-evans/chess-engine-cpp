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

    uint16_t start_square_index;

    sf::Texture texture;
    sf::Sprite sprite;

    Color color;

    sf::RenderWindow& window;

    std::vector<Pos> piece_positions;

    std::string piece_id;

    std::string resolve_texture_path();

public:
    sf::Vector2f pos;

    Piece(std::string id, Color col, sf::RenderWindow& w, uint16_t squ_idx, int b_squ_sz);

    virtual std::string get_texture_path();

    void draw(sf::RenderWindow& window);

    void init_piece_positions_vector_from_bitboard(uint64_t bitboard, std::vector<Pos> &piece_pos);
    void make_highlighted();
    void render_highlight(sf::Vector2f clicked_pos, std::vector<sf::RectangleShape>& squares, bool& piece_highlight_active);

    bool move_square_has_friendly_piece(Pos piece_pos, sf::Vector2f move);
    virtual void get_legal_moves(Pos pos) = 0;
};

class Pawn : public Piece {
public:
    Pawn(std::string id, Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("P", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
    void get_legal_moves(Pos pos) override;

};

class Knight : public Piece {
public:
    const int offsets[8][2] = {
        {-2, -1}, {-2, 1}, {2, -1}, {2, 1},
        {-1, -2}, {1, -2}, {-1, 2}, {1, 2}
    };

    Knight(std::string id, Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("N", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
    
    bool is_knight_move_on_board(Pos piece_square, int dx, int dy);

    void get_legal_moves(Pos pos) override;
};

class Bishop : public Piece {
public:
    Bishop(std::string id, Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("B", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
    
    void get_legal_moves(Pos pos) override;

};

class Rook : public Piece {
public:
    Rook(std::string id, Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("R", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }

    void get_legal_moves(Pos pos) override;

};

class Queen : public Piece {
public:
    Queen(std::string id, Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("Q", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }

    void get_legal_moves(Pos pos) override;

};

class King : public Piece {
public:
    King(std::string id, Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("K", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() override {
        return resolve_texture_path();
    }
    void get_legal_moves(Pos pos) override;

};