#pragma once

class Player {

public:
    Player(const bool is_w_side);

    bool is_white_side;

    bool is_white();
    virtual bool is_human() = 0;
};

class Human : public Player {
public:
    Human(const bool is_w_side) : 
    Player(is_w_side) {}

    bool is_human() override {return true;};
};

class Engine : public Player {
public:
    Engine(const bool is_w_side) : 
    Player(is_w_side) {}

    bool is_human() override {return false;};
};