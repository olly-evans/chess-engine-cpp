
#include "player.hpp"

Player::Player(const bool is_w_side) :
is_white_side(is_w_side) {};

bool Player::is_white() {
    return is_white_side;
}