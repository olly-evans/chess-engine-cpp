#pragma once

#include <memory>

#include "SFML/Graphics.hpp"

#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH 1280

#define WINDOW_NAME "Chess"

class Board {

private:

    /* WINDOW */
    const unsigned int win_h;
    const unsigned int win_w;
    const std::string win_name;

    sf::RenderWindow window;
    
    /* GRID */
    unsigned int board_square_size;
    static constexpr unsigned int GRID_SZ = 8;
    static constexpr unsigned int GRID_NUM_SQUARES = GRID_SZ * GRID_SZ;

    std::vector<sf::RectangleShape> squares;   

    // Bitboards.



public:

    Board(const unsigned int ww, const unsigned int wh, const std::string wn) : 
    win_w(ww), win_h(wh), win_name(wn), window(sf::VideoMode(win_w, win_h), win_name){};

    /* UTIL METHODS */

    void die(std::string err) {
        window.close();
        std::cerr << err << std::endl;
        exit(1);
    }

    sf::Vector2f index_to_2d(int i) {
        return sf::Vector2f(i % GRID_SZ, i / GRID_SZ);
    }
    
    bool is_square_white(int i) {
        sf::Vector2f xy = index_to_2d(i);
        return ((int)xy.x + (int)xy.y) % 2;
    }

    /* INIT */

    void init() {
        
        init_get_board_square_size(&board_square_size, win_h, win_w);
        init_board_squares(squares);

        // init_board_startpos();

    }

    void init_get_board_square_size(unsigned int *sz, const unsigned win_h, const unsigned win_w) {
        if ((win_h % GRID_SZ) != 0 | (win_w % GRID_SZ) != 0) die("Window size must support eight squares.");
        if (!(win_w == win_h)) die("Window must be square!");

        *sz = win_h / GRID_SZ;
    }

    void init_board_squares(std::vector<sf::RectangleShape> squ) {

        for (int i = 0; i < GRID_NUM_SQUARES; i++) {

            sf::Vector2f pos = index_to_2d(i) * (float)board_square_size;

            // because we put at the back, top left square is black not white.
            // something along these lines.
            squares.emplace_back(sf::Vector2f(board_square_size, board_square_size));

            squares[i].setPosition(pos);
            squares[i].setFillColor(is_square_white(i) ? sf::Color::White : sf::Color::Black);
        }   
    }

    void render() {
        window.clear();
        for (auto& squ : squares) {window.draw(squ);}
        window.display();
    }

    void run() {

        sf::Event event;
        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
            }
        }
    };
};