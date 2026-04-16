#pragma once

#include "util.hpp"

// board.hpp needs to include piece.hpp to instantiate pieces.
// This needed to let pieces.hpp know that Board exists.

class Board;

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

    Board &board;

    // assert king/queen piece_pos.size() == 1.

public:
    Piece(Board &board, Color col, sf::RenderWindow& w, uint64_t bitboard, int b_squ_sz) : 
    board(board), color(col), window(w), board_square_size(b_squ_sz){
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
    Pawn(Board &board, Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(board, col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {

        // if ((Board::get_win_w_h() / GRID_SZ)
        
        // choose a size based on the win_h and win_w, which are equal.
        // we choose the size that is closest to board.get_win_w_h() / GRID_SZ;

        if (board.board_square_size > board.TEXTURE_SIZE_320) {

        }

        // format string to be appropriate size.
        return color == Color::WHITE ? "assets/wP_320px.png" : "assets/bP_320px.png";  
    };
};

class Bishop:public Piece {
public:
    Bishop(Board &board, Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(board, col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wB_320px.png" : "assets/bB_320px.png";  
    };
};

class Knight:public Piece {
public:
    Knight(Board &board, Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(board, col, w, bitboard, board_square_size) {}
    
    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wN_320px.png" : "assets/bN_320px.png";  
    };
};

class Rook:public Piece {
public:
    Rook(Board &board, Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(board, col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wR_320px.png" : "assets/bR_320px.png";  
    };
};

class Queen:public Piece {

public:
    Queen(Board &board, Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(board, col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wQ_320px.png" : "assets/bQ_320px.png";  
    };
};

class King:public Piece {
public:
    King(Board &board, Color col, sf::RenderWindow& w, uint64_t bitboard, int board_square_size) : 
    Piece(board, col, w, bitboard, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wK_320px.png" : "assets/bK_320px.png";  
    };
};