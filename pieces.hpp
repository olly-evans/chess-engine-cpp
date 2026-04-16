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

public:
    Piece(Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
    color(col), window(w), board_square_size(b_squ_sz){
        bitboard_to_piece_pos(bitboard, piece_positions);
    }

    void draw(sf::RenderWindow& window) {

        texture.loadFromFile(get_texture_path());
        sprite.setTexture(texture);

        for (int i = 0; i < piece_positions.size(); i++) {
            sprite.setPosition(piece_positions[i]);
            window.draw(sprite);
        }
    }

    virtual std::string get_texture_path() = 0;

    void bitboard_to_piece_pos(uint64_t bitboard, std::vector<sf::Vector2f> &piece_pos) {

        for (int i = 0; i < GRID_NUM_SQUARES; i++) {
            if (bitboard & (1ULL << i)) {
                int draw_idx = GRID_NUM_SQUARES - i - 1;
                piece_pos.push_back(index_to_2d(draw_idx) * (float)board_square_size);
            }
        }
    }

};

class Pawn:public Piece {
public:
    Pawn(Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {

        int diff_from_80px= abs(board_square_size- TEXTURE_SIZE_80);
        int diff_from_160px = abs(board_square_size - TEXTURE_SIZE_160);
        int diff_from_320px = abs(board_square_size - TEXTURE_SIZE_320);

        int min_diff = std::min({diff_from_80px, diff_from_160px, diff_from_320px});

        if (min_diff == diff_from_80px) return color == Color::WHITE ? "assets/wP_80px.png" : "assets/bP_80px.png";
        if (min_diff == diff_from_160px) return color == Color::WHITE ? "assets/wP_160px.png" : "assets/bP_160px.png";
        if (min_diff == diff_from_320px) return color == Color::WHITE ? "assets/wP_320px.png" : "assets/bP_320px.png";




        // format string to be appropriate size.
        // return color == Color::WHITE ? "assets/wP_320px.png" : "assets/bP_320px.png";  
    };
};

class Bishop:public Piece {
public:
    Bishop(Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wB_320px.png" : "assets/bB_320px.png";  
    };
};

class Knight:public Piece {
public:
    Knight(Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(col, w, bitboard, board_square_size) {}
    
    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wN_320px.png" : "assets/bN_320px.png";  
    };
};

class Rook:public Piece {
public:
    Rook(Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wR_320px.png" : "assets/bR_320px.png";  
    };
};

class Queen:public Piece {

public:
    Queen(Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wQ_320px.png" : "assets/bQ_320px.png";  
    };
};

class King:public Piece {
public:
    King(Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wK_320px.png" : "assets/bK_320px.png";  
    };
};