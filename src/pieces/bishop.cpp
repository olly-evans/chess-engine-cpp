
#include "bitboardhelper.hpp"
#include "pieces/bishop.hpp"

void Bishop::set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb, uint8_t castling_rights) {
    uint64_t bishop = (1ULL << this->bit);
    uint64_t moves = 0ULL;

    moves |= get_north_west_moves(bishop, w_bb, b_bb);
    moves |= get_north_east_moves(bishop, w_bb, b_bb);
    moves |= get_south_west_moves(bishop, w_bb, b_bb);
    moves |= get_south_east_moves(bishop, w_bb, b_bb);

    uint64_t enemy = (this->is_white) ? b_bb : w_bb; 
    this->captures = (moves & enemy);

    moves = BBHelper::remove_friendly_pieces(moves, (this->is_white ? w_bb : b_bb)); 
    this->moves = BBHelper::remove_enemy_pieces(moves, (this->is_white) ? b_bb : w_bb);
};

void Bishop::strip_pseudo_legal_special_moves(Board& board) {
    return;
}