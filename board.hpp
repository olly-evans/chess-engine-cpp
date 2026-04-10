#pragma once

#include "SFML/Graphics.hpp"

#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH 1280

#define WINDOW_NAME "Chess"

class Board {

private:
    const unsigned int win_h;
    const unsigned int win_w;
    const std::string win_name;
    sf::RenderWindow window;
    
    unsigned int board_square_size;
    static constexpr unsigned int GRID_SZ = 8;
    static constexpr unsigned int GRID_NUM_SQUARES = GRID_SZ * GRID_SZ;

    std::vector<sf::RectangleShape> squares[GRID_NUM_SQUARES];

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
        
        init_board_square_size(&board_square_size, win_h, win_w);
        // init_board_squares();
        // init_board_start();

    }

    const unsigned int *init_board_square_size(unsigned int *sz, const unsigned win_h, const unsigned win_w) {
        if (!(win_w == win_h)) die("Window must be square!");
        *sz = win_h / GRID_SZ;
        if ((win_h % GRID_SZ) != 0) die("Window size must support eight squares.");
        return sz;
    }

    void run() {
        sf::RectangleShape square(sf::Vector2f(300.f, 300.f)); // width, height
        square.setPosition(0.f, 0.f);                        // x, y
        square.setFillColor(sf::Color::Red);


        sf::Event event;
        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
            }
            window.clear();
            window.draw(square);
            window.display();
        }
    };
};

