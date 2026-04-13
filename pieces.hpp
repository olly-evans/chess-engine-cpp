#pragma once

#include "board.hpp"

class Piece{
protected:
    int square_index;
    int x,y;

    sf::Texture texture;
    sf::Sprite sprite;

    sf::RenderWindow& window;



public:
    Piece(sf::RenderWindow& w) : window(w) {}

    virtual void draw(sf::RenderWindow& window) = 0;

};

class Bishop:public Piece {

public:

    Bishop(sf::RenderWindow& w) : Piece(w) {}

    void draw(sf::RenderWindow& window) {
        sf::Texture texture;
        texture.loadFromFile("assets/bK.png");
        sf::Sprite sprite(texture);
        sprite.setPosition(sf::Vector2f(2, 2));
        window.draw(sprite);
    };
};