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

public:
    Piece(Color col, sf::RenderWindow& w, int squ_idx, int b_squ_sz) : 
    color(col), window(w), square_index(squ_idx), board_square_size(b_squ_sz){}

    void draw(sf::RenderWindow& window) {

        texture.loadFromFile(get_texture_path());
        sf::Vector2f pos = index_to_2d(square_index);

        // get xy() of piece.
        
        sprite.setTexture(texture);
        sprite.setPosition(pos * (float)board_square_size);
        window.draw(sprite);
    }
    virtual std::string get_texture_path() = 0;
    //set x,y
    //get x,y
};

class Pawn:public Piece {
public:
    Pawn(Color col, sf::RenderWindow& w, int squ_idx, int board_square_size) : 
    Piece(col, w, squ_idx, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wP.png" : "assets/bP.png";  
    };
};

class Bishop:public Piece {
public:
    Bishop(Color col, sf::RenderWindow& w, int squ_idx, int board_square_size) : 
    Piece(col, w, squ_idx, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wB.png" : "assets/bB.png";  
    };
};

class Knight:public Piece {
public:
    Knight(Color col, sf::RenderWindow& w, int squ_idx, int board_square_size) : 
    Piece(col, w, squ_idx, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wN.png" : "assets/bN.png";  
    };
};

class Rook:public Piece {
public:
    Rook(Color col, sf::RenderWindow& w, int squ_idx, int board_square_size) : 
    Piece(col, w, squ_idx, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wR.png" : "assets/bR.png";  
    };
};

class Queen:public Piece {

public:
    Queen(Color col, sf::RenderWindow& w, int squ_idx, int board_square_size) : 
    Piece(col, w, squ_idx, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wQ.png" : "assets/bQ.png";  
    };
};

class King:public Piece {
public:
    King(Color col, sf::RenderWindow& w, int squ_idx, int board_square_size) : 
    Piece(col, w, squ_idx, board_square_size) {}

    std::string get_texture_path() override {
        return color == Color::WHITE ? "assets/wK.png" : "assets/bK.png";  
    };
};