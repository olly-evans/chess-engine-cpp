#pragma once

#include "board.hpp"

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

        int x,y;
        // get x and y in vector2f from square index

        // give to setPosition
        
        sprite.setTexture(texture);
        sprite.setPosition(sf::Vector2f(0, 0));
        window.draw(sprite);
    };
};