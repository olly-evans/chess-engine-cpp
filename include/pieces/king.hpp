#pragma once

#include "piece.hpp"

class King : public Piece {
private:

    bool can_kingside_castle = false;
    bool can_queenside_castle = false;

public:
    King(char id, uint8_t b) : 
        Piece(id, b) {}

    void set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb, uint8_t castling_rights) override;
    
    void strip_pseudo_legal_special_moves(Board& board) override;

    void update_castling_rights(uint8_t castling_rights);

};