#pragma once

class Player {

protected:
    Player(const bool is_w_side);

    bool is_white_side;

    bool is_white();
    bool is_human();
};

class Human : public Player {
public:
    Human(const bool is_w_side) : 
    Player(is_w_side) {}
};

// class Engine : public Player {
// public:
//     Engine(const bool is_w_side) : 
//     Player(is_w_side) {}
// };