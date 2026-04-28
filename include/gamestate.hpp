#pragma once

#include <iostream>

#include "player.hpp"


class GameState {

/* Skeleton for GameState class and things from board I want to extract. */

public:
    bool is_whites_turn = true;

    // init via gui eventually as a load position option.
    std::string fen_pos;

    Player* p_white;
    Player* p_black;

    enum State {
        WHITE_TURN,
        BLACK_TURN,
        WHITE_CHECKMATE,
        BLACK_CHECKMATE,
        STALEMATE
    };

    void get_fen_pos();
};