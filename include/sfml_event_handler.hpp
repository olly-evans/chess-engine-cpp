#pragma once

#include "SFML/Graphics.hpp"


class SFMLEventHandler {
private:
    sf::RenderWindow& main_window;

public:
    SFMLEventHandler(sf::RenderWindow& main_win);
};