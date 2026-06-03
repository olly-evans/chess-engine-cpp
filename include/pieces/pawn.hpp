#pragma once

#include <cstdint>

#include "piece.hpp"

class Pawn : public Piece {
public:
    Pawn(char id, uint8_t bitboard) : 
        Piece(id, bitboard) {}
    
    uint64_t en_passant_capture_bit = 0ULL;
    uint64_t enpassant_from_fen = 0ULL;

    void set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb, uint8_t castling_rights) override;

    void strip_pseudo_legal_special_moves(Board& board) override;

    uint64_t get_white_pawn_moves(uint64_t pawn, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_black_pawn_moves(uint64_t pawn, uint64_t w_bb, uint64_t b_bb);

    uint64_t get_enpassant(uint64_t w_bb, uint64_t b_bb);
};