
#include "pieces/king.hpp"
#include "piece.hpp"
#include "bitboardhelper.hpp"
#include "fenparser.hpp"
#include "board.hpp"

void King::set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) {

    uint64_t king = (1ULL << this->bit);
    uint64_t moves = 0ULL;
    bool is_white = (isupper(this->id));

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

    uint64_t enemy_occupancy = (is_white) ? b_bb : w_bb; 
    this->captures = (moves & enemy_occupancy);

    // uint64_t enemy_captures = (is_white) ? Board::black_captures() : Board::white_captures();

    // black.
    if (this->can_queenside_castle) 
        moves |= (king << 2);
    if (this->can_kingside_castle) 
        moves |= (king >> 2);

    moves = BBHelper::remove_friendly_pieces(moves, is_white ? w_bb : b_bb);
    this->moves = BBHelper::remove_enemy_pieces(moves, is_white ? b_bb : w_bb);
};

void King::strip_pseudo_legal_special_moves(Board& board) {
    return;
}

void King::update_castling_rights(Board& board) {

    if (board.castling_rights == 0)
        return; 

    if (this->is_white) {

        this->can_kingside_castle = (board.castling_rights & 8) ? true : false;

        this->can_queenside_castle = (board.castling_rights & 4) ? true : false;
    }

    this->can_kingside_castle = (board.castling_rights & 2) ? true : false;
    this->can_queenside_castle = (board.castling_rights & 1) ? true : false;
}