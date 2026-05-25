#pragma once

#include "pieces.hpp"

class Queen : public Piece {
public:
    Queen(char id, uint8_t b) : 
        Piece(id, b) {}

    void set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) override;

    void strip_pseudo_legal_special_moves(Board& board) override;
};