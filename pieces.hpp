#pragma once

#include "util.hpp"

#include <iostream>
#include <filesystem>

enum class Color {BLACK, WHITE};

class Piece {

protected:
    int board_square_size;

    sf::Texture texture;
    sf::Sprite sprite;

    const unsigned int TEXTURE_SIZE_80 = 80;
    const unsigned int TEXTURE_SIZE_160 = 160;
    const unsigned int TEXTURE_SIZE_320 = 320;

    Color color;

    sf::RenderWindow& window;

    uint64_t bitboard;
    std::vector<sf::Vector2f> piece_positions;
    // assert king/queen piece_pos.size() == 1.

    std::string piece_id;

    std::string resolve_texture_path() {

        std::string size_suffix;
        if (board_square_size >= TEXTURE_SIZE_320) {
            size_suffix = "320px";
        } else if (board_square_size >= TEXTURE_SIZE_160) {
            size_suffix = "160px";
        } else {
            size_suffix = "80px";
        }

        std::filesystem::path path = std::filesystem::current_path();
        std::string color_prefix = (color == Color::WHITE ? "w" : "b");
        return path.string() + "/assets/" + color_prefix + piece_id + "_" + size_suffix + ".png";
    }

public:
    Piece(std::string id, Color col, sf::RenderWindow& w, uint64_t bitb, int b_squ_sz) : 
        piece_id(std::move(id)), 
        color(col), 
        window(w), 
        bitboard(bitb),
        board_square_size(b_squ_sz) {
            
        init_piece_positions_vector_from_bitboard(bitboard, piece_positions);
        if (texture.loadFromFile(get_texture_path())) sprite.setTexture(texture);
    }

    virtual std::string get_texture_path() {
        return resolve_texture_path();
    }

    void draw(sf::RenderWindow& window) {
        // Calculate the gap between the square size and the actual image size
        sf::FloatRect sprite_size = sprite.getGlobalBounds();
        float offset_x = (board_square_size - sprite_size.width) / 2.0f;
        float offset_y = (board_square_size - sprite_size.height) / 2.0f;

        for (const auto& pos : piece_positions) {
            // Apply the offset to the top-left square coordinate
            sprite.setPosition(pos.x + offset_x, pos.y + offset_y);
            window.draw(sprite);
        }
    }

    void init_piece_positions_vector_from_bitboard(uint64_t bitboard, std::vector<sf::Vector2f> &piece_pos) {

        // init_positions_vector_from_bitboard();
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