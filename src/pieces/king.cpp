
#include "pieces/king.hpp"
#include "pieces.hpp"
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

    bool can_qs_castle = can_pseudo_legal_queenside_castle(w_bb, b_bb);

    if (can_qs_castle) 
        moves |= (king << 2);

    moves = BBHelper::remove_friendly_pieces(moves, is_white ? w_bb : b_bb);
    this->moves = BBHelper::remove_enemy_pieces(moves, is_white ? b_bb : w_bb);
};

bool King::can_pseudo_legal_queenside_castle(uint64_t w_bb, uint64_t b_bb) {


    // scrapping this, board holds QqKk, pop appropriate char when piece->has_moved and other conditions met etc.

    /*
    enum CastlingRights {
        W_KINGSIDE  = 1 << 0,
        W_QUEENSIDE = 1 << 1,
        B_KINGSIDE  = 1 << 2,
        B_QUEENSIDE = 1 << 3,
    };

    uint8_t castling_rights = W_KINGSIDE | W_QUEENSIDE | B_KINGSIDE | B_QUEENSIDE;
    
    this is in board, figure it out from this this is how decent engines do it apparently no has_moved needed.
    do need to figure out how to append to ->moves though.

    */
    uint64_t king = (1ULL << this->bit);
    uint64_t rooks = (this->is_white) ? Board::bitboards[FenParser::W_ROOKS] : Board::bitboards[FenParser::B_ROOKS];

    uint64_t king_start = (this->is_white) ? 0x8 : 0x800000000000000;
    uint64_t queenside_rook_start = (this->is_white) ? 0x80 : 0x8000000000000000;


    // if (this->has_moved)
    //     return false;

    if (!(king_start & king))
        return false;

    // is queenside flag. problem is i cant access the rook from here.
    // This will work with either rook is the problem. And also if the rook has moved back onto square.
    
    if (!(rooks & queenside_rook_start))
        return false;
    
    uint64_t occupancy = w_bb | b_bb;
    bool offset_one = !(occupancy & (king << 1));
    bool offset_two = !(occupancy & (king << 2));

    if (!(offset_one && offset_two))
        return false;


    // rook pointer has_moved somehow idk.
    return true;
}

void King::strip_pseudo_legal_special_moves(Board& board) {
    return;
}