
#include "pieces/king.hpp"
#include "piece.hpp"

#include "bitboardhelper.hpp"
#include "fenparser.hpp"
#include "board.hpp"


void King::set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb, uint8_t castling_rights) {

    uint64_t king = (1ULL << this->bit);
    uint64_t moves = 0ULL;

    update_castling_rights(castling_rights);

    moves |= (king << 1);
    moves |= (king << 7);
    moves |= (king << 8);
    moves |= (king << 9);

    moves |= (king >> 1);
    moves |= (king >> 7);
    moves |= (king >> 8);
    moves |= (king >> 9);

    if (this->file == 7) moves &= ~BBHelper::file_masks[this->file];
    if (this->file == 0) moves &= ~BBHelper::file_masks[this->file];

    uint64_t enemy_occupancy = (this->is_white) ? b_bb : w_bb; 
    this->captures = (moves & enemy_occupancy);

    // append the specific squares if so not this bs.
    // BBHelper::square_name_to_bit()
    // if (this->can_queenside_castle) 
    //     moves |= (king << 2);
    // if (this->can_kingside_castle) 
    //     moves |= (king >> 2);
    
    if (this->is_white) {
        if (this->can_kingside_castle) moves |= (1ULL << BBHelper::square_name_to_bit("g1"));
        if (this->can_queenside_castle) moves |= (1ULL << BBHelper::square_name_to_bit("c1"));
    } else {
        if (this->can_kingside_castle) moves |= (1ULL << BBHelper::square_name_to_bit("g8"));
        if (this->can_queenside_castle) moves |= (1ULL << BBHelper::square_name_to_bit("c8"));
    }

    moves = BBHelper::remove_friendly_pieces(moves, is_white ? w_bb : b_bb);
    this->moves = BBHelper::remove_enemy_pieces(moves, is_white ? b_bb : w_bb);
};

void King::strip_pseudo_legal_special_moves(Board& board) {
    return;
}

void King::update_castling_rights(uint8_t castling_rights) {

    if (castling_rights == 0) {
        this->can_kingside_castle = false;
        this->can_queenside_castle = false;
        return;
    }

    if (this->is_white) {
        this->can_kingside_castle = (castling_rights & (1 << 3)) ? true : false;
        this->can_queenside_castle = (castling_rights & (1 << 2)) ? true : false;
        return;
    }

    this->can_kingside_castle = (castling_rights & (1 << 1)) ? true : false;
    this->can_queenside_castle = (castling_rights & (1 << 0)) ? true : false;
}