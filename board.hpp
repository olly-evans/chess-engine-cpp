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

    void die(std::string err) {
        window.close();
        std::cerr << err << std::endl;
        exit(1);
    }

    void init() {
        
        init_get_board_square_size(&board_square_size, win_h, win_w);
        init_board_squares(squares);
        // init_board_squares(squares);
        // draw_board_squares();

        // init_board_startpos();

    }

    void init_get_board_square_size(unsigned int *sz, const unsigned win_h, const unsigned win_w) {
        if ((win_h % GRID_SZ) != 0 | (win_w % GRID_SZ) != 0) die("Window size must support eight squares.");
        if (!(win_w == win_h)) die("Window must be square!");

        *sz = win_h / GRID_SZ;
    }

    void init_board_squares(std::vector<sf::RectangleShape> squ) {

        for (int i = 0; i < GRID_NUM_SQUARES; i++) {
            float x = (i % GRID_SZ) * board_square_size;
            float y = (i / GRID_SZ) * board_square_size;

            squares.emplace_back(sf::Vector2f(board_square_size, board_square_size));
            squares[i].setPosition(sf::Vector2f(x, y));
            squares[i].setFillColor(sf::Color::White);
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