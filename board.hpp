#pragma once

#define WINDOW_HEIGHT 1280
#define WINDOW_WIDTH 1280
#define WINDOW_NAME "Chess"

class Board {

private:
    // bitboards.
    
public:
    void smfl_window_init() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME);
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

