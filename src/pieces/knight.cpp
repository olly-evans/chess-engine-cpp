
#include "bitboardhelper.hpp"

#include "pieces/knight.hpp"

void Knight::set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb, uint8_t castling_rights) {
    
    uint64_t knight = 1ULL << this->bit;
    uint64_t moves = 0ULL;

    moves |= (knight & BBHelper::NOT_AB_FILE) << 6;
    moves |= (knight & BBHelper::NOT_A_FILE)  << 15; 
    moves |= (knight & BBHelper::NOT_H_FILE)  << 17;
    moves |= (knight & BBHelper::NOT_GH_FILE) << 10;
    
    moves |= (knight & BBHelper::NOT_GH_FILE) >> 6;   
    moves |= (knight & BBHelper::NOT_H_FILE)  >> 15;  
    moves |= (knight & BBHelper::NOT_A_FILE)  >> 17; 
    moves |= (knight & BBHelper::NOT_AB_FILE) >> 10; 

    uint64_t enemy = (this->is_white) ? b_bb : w_bb; 
    this->captures = (moves & enemy);

    moves = BBHelper::remove_friendly_pieces(moves, (this->is_white ? w_bb : b_bb)); 
    this->moves = BBHelper::remove_enemy_pieces(moves, (this->is_white) ? b_bb : w_bb);
}

void Knight::strip_pseudo_legal_special_moves(Board& board) {
    return;
}