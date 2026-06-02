#pragma once

#include "piece.hpp"

class King : public Piece {
private:

    bool can_kingside_castle;
    bool can_queenside_castle;

public:
    King(char id, uint8_t b) : 
        Piece(id, b) {}

    void set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) override;
    
    void strip_pseudo_legal_special_moves(Board& board) override;

    void update_castling_rights(Board& board);

};