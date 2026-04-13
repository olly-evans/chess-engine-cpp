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

    virtual void draw(sf::RenderWindow& window) = 0;

    //set x, y
    //get x,y
};

class Bishop:public Piece {

public:

    Bishop(Color col, sf::RenderWindow& w, int squ_idx, int board_square_size) : 
    Piece(col, w, squ_idx, board_square_size) {}

    void draw(sf::RenderWindow& window) {

        if (color == Color::WHITE)
            texture.loadFromFile("assets/wB.png");
        else
            texture.loadFromFile("assets/bB.png");

        sf::Vector2f pos = index_to_2d(square_index);
        
        sprite.setTexture(texture);
        sprite.setPosition(pos * (float)board_square_size);
        window.draw(sprite);
    };
};