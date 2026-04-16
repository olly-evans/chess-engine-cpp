#pragma once

#include "board.hpp"
#include "util.hpp"

enum class Color {BLACK, WHITE};

class Piece {

protected:
    int square_index;
    int board_square_size;
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
        return color == Color::WHITE ? "assets/wP_160px.png" : "assets/bP_160px.png";  
    };
};

class Bishop:public Piece {
public:
    Bishop(Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wB_160px.png" : "assets/bB_160px.png";  
    };
};

class Knight:public Piece {
public:
    Knight(Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wN_160px.png" : "assets/bN_160px.png";  
    };
};

class Rook:public Piece {
public:
    Rook(Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wR_160px.png" : "assets/bR_160px.png";  
    };
};

class Queen:public Piece {

public:
    Queen(Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wQ_160px.png" : "assets/bQ_160px.png";  
    };
};

class King:public Piece {
public:
    King(Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wK_160px.png" : "assets/bK_160px.png";  
    };
};