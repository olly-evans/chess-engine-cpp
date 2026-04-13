#pragma once

#include "board.hpp"
#include "util.hpp"

enum class Color {BLACK, WHITE};

class Piece {

protected:
    int square_index;
    int x,y;

    Color color;

    sf::Texture texture;
    sf::Sprite sprite;

    sf::RenderWindow& window;

public:
    Piece(Color col, sf::RenderWindow& w, int squ_idx) : color(col), window(w), square_index(squ_idx) {}

    virtual void draw(sf::RenderWindow& window) = 0;
};

class Bishop:public Piece {

public:

    Bishop(Color col, sf::RenderWindow& w, int squ_idx) : Piece(col, w, squ_idx) {}

    void draw(sf::RenderWindow& window) {

        if (color == Color::WHITE)
            texture.loadFromFile("assets/wB.png");
        else
            texture.loadFromFile("assets/bB.png");

        sf::Vector2f pos = index_to_2d(square_index);
        
        sprite.setTexture(texture);
        sprite.setPosition(pos);
        window.draw(sprite);
    };
};