#pragma once

#include "piece.hpp"  

class Rook : public Piece {
public:
    Rook(char id, uint8_t b) : 
        Piece(id, b) {}

    void set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb, uint8_t castling_rights) override;
    
    void strip_pseudo_legal_special_moves(Board& board) override;
};