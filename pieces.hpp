#pragma once

#include "util.hpp"


enum class Color {BLACK, WHITE};

class Piece {

protected:
    int square_index;
    int board_square_size;

    const unsigned int TEXTURE_SIZE_80 = 80;
    const unsigned int TEXTURE_SIZE_160 = 160;
    const unsigned int TEXTURE_SIZE_320 = 320;

    int x,y;

    Color color;

    sf::Texture texture;
    sf::Sprite sprite;

    sf::RenderWindow& window;

    std::vector<sf::Vector2f> piece_positions;
    // assert king/queen piece_pos.size() == 1.

    std::string piece_id; // Store "P", "K", "R", etc.
    // ... other variables ...

    std::string resolve_texture_path() {
        int diff_80  = abs(board_square_size - (int)TEXTURE_SIZE_80);
        int diff_160 = abs(board_square_size - (int)TEXTURE_SIZE_160);
        int diff_320 = abs(board_square_size - (int)TEXTURE_SIZE_320);

        int min_diff = std::min({diff_80, diff_160, diff_320});

        std::string size_suffix;
        if (min_diff == diff_80) size_suffix = "80px";
        else if (min_diff == diff_160) size_suffix = "160px";
        else size_suffix = "320px";

        std::string color_prefix = (color == Color::WHITE ? "w" : "b");
        
        return "assets/" + color_prefix + piece_id + "_" + size_suffix + ".png";
    }

public:
    Piece(std::string id, Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        piece_id(id), color(col), window(w), board_square_size(b_squ_sz) {
        bitboard_to_piece_pos(bitboard, piece_positions);
    }

    virtual std::string get_texture_path() {
        return resolve_texture_path();
    }

    void draw(sf::RenderWindow& window) {

        texture.loadFromFile(get_texture_path());
        sprite.setTexture(texture);

        for (int i = 0; i < piece_positions.size(); i++) {
            sprite.setPosition(piece_positions[i]);
            window.draw(sprite);
        }
    }

    void bitboard_to_piece_pos(uint64_t bitboard, std::vector<sf::Vector2f> &piece_pos) {

        for (int i = 0; i < GRID_NUM_SQUARES; i++) {
            if (bitboard & (1ULL << i)) {
                int draw_idx = GRID_NUM_SQUARES - i - 1;
                piece_pos.push_back(index_to_2d(draw_idx) * (float)board_square_size);
            }
        }
    }
};

class Pawn : public Piece {
public:
    Pawn(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("P", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() {
        return resolve_texture_path();
    }
};

class Knight : public Piece {
public:
    Knight(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("N", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() {
        return resolve_texture_path();
    }
    
};

class Bishop : public Piece {
public:
    Bishop(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("B", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() {
        return resolve_texture_path();
    }
};

class   Rook : public Piece {
public:
    Rook(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("R", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() {
        return resolve_texture_path();
    }
};

class   Queen : public Piece {
public:
    Queen(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("Q", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() {
        return resolve_texture_path();
    }
};

class King : public Piece {
public:
    King(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
        Piece("K", col, w, bitboard, b_squ_sz) {}

    virtual std::string get_texture_path() {
        return resolve_texture_path();
    }
};