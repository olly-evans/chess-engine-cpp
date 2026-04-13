#pragma once

#include "board.hpp"

enum class Color {BLACK, WHITE};

class Piece{

protected:
    int square_index;
    int x,y;

    Color color;

    sf::Texture texture;
    sf::Sprite sprite;

    sf::RenderWindow& window;



public:
    Piece(Color col, sf::RenderWindow& w) : color(col), window(w) {}

    virtual void draw(sf::RenderWindow& window) = 0;

};

class Bishop:public Piece {

public:

    Bishop(Color col, sf::RenderWindow& w) : Piece(col, w) {}

    void draw(sf::RenderWindow& window) {
        sf::Texture texture;
        texture.loadFromFile("assets/bB.png");
        sf::Sprite sprite(texture);
        sprite.setPosition(sf::Vector2f(0, 0));
        window.draw(sprite);
    };
};