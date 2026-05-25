
#include "pieces/pawn.hpp"
#include "bitboardhelper.hpp"
#include "board.hpp"
#include "movelogger.hpp"

void Pawn::set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) {


    uint64_t pawn = 1ULL << this->bit;
    uint64_t moves = 0ULL;

    if (this->is_white) {
        moves = get_white_pawn_moves(pawn, w_bb, b_bb);

        // enpassant

        this->captures |= get_enpassant(w_bb, b_bb);
        // promotions.
    } else {


        moves = get_black_pawn_moves(pawn , w_bb, b_bb);
        this->captures |= get_enpassant(w_bb, b_bb);
    }

    moves = BBHelper::remove_friendly_pieces(moves, b_bb);
    this->moves = BBHelper::remove_enemy_pieces(moves, w_bb);
};

uint64_t Pawn::get_white_pawn_moves(uint64_t pawn, uint64_t w_bb, uint64_t b_bb) {
    
    /* White pawns as of right now will always march in the northern direction. */

    uint64_t moves = 0ULL;
    uint64_t captures = 0ULL;

    uint64_t white_pawn_start_rank = BBHelper::rank_masks[1];

    if (b_bb & (pawn << 9)) 
        captures |= ((pawn & ~BBHelper::file_masks[7]) << 9);

    if (b_bb & (pawn << 7)) 
        captures |= ((pawn & ~BBHelper::file_masks[0]) << 7);

    
    this->captures = captures | this->enpassant_from_fen; 
    if (this->has_moved)
        this->enpassant_from_fen = 0ULL;

    if (w_bb & (pawn << 8) | b_bb & (pawn << 8)) 
        return moves;
    moves |= (pawn << 8);

    // Or doesn't seem right
    if (!(pawn & (white_pawn_start_rank)) | (b_bb & (pawn << 16))) return moves;
    moves |= (pawn << 16);

    return moves;
}

uint64_t Pawn::get_black_pawn_moves(uint64_t pawn, uint64_t w_bb, uint64_t b_bb) {
    
    /* Black pawns as of right now will always march in the southern direction. */

    uint64_t moves = 0ULL;
    uint64_t captures = 0ULL;

    uint64_t black_pawn_start_rank = BBHelper::rank_masks[6];

    // Find the captures, mask out ones that overlap to next file.
    if (w_bb & (pawn >> 9)) 
        captures = (pawn & ~BBHelper::file_masks[0]) >> 9;

    if (w_bb & (pawn >> 7)) 
        captures = (pawn & ~BBHelper::file_masks[7]) >> 7;
    
    // enpassent_from_fen non-zero with a bit set if we have an enpassant square in the fen string.
    // first turn only of course, doesnt need to be reset i dont think, would be funny if pawn promotes and we cahnge the piece type though.
    this->captures = captures | this->enpassant_from_fen; 

    if (this->has_moved)
        this->enpassant_from_fen = 0ULL;

    if (w_bb & (pawn >> 8) | b_bb & (pawn >> 8)) return moves;
    moves |= (pawn >> 8);

    // The OR doesn't seem right but works so eh.
    if (!(pawn & (black_pawn_start_rank)) | (w_bb & (pawn >> 16))) return moves;
    moves |= (pawn >> 16);
    
    return moves;
}

uint64_t Pawn::get_enpassant(uint64_t w_bb, uint64_t b_bb) {

    uint64_t en_passant_moves = 0ULL;

    uint64_t pawn = (1ULL << this->bit);

    uint64_t enemy_pawns = this->is_white ? Board::bitboards[FenParser::B_PAWNS] : Board::bitboards[FenParser::W_PAWNS];

    uint64_t west = (pawn << 1);
    uint64_t east = (pawn >> 1);

    if (!(enemy_pawns & west) && !(enemy_pawns & east))
        return 0ULL;

    if (MoveLogger::move_history.empty())
        return 0ULL;

    Move& last_move = MoveLogger::move_history.back();

    // true in our instance.
    bool moved_two = (abs(last_move.end_bit - last_move.start_bit) == 16);

    
    bool east_moved_two = moved_two && (east == (1ULL << last_move.end_bit));
    bool west_moved_two = moved_two && (west == (1ULL << last_move.end_bit));

    // this->enpassant_captures, shows the capture square
    // move to clicked bit, remove clicked_bit << 8, clicked_bit >> 8. in board.

    if (enemy_pawns & west && west_moved_two && this->is_white) {
        en_passant_moves |= (west << 8);
        this->en_passant_capture_bit |= west;
    }

    if (enemy_pawns & east && east_moved_two && this->is_white) {
        en_passant_moves |= (east << 8);
        this->en_passant_capture_bit |= east;
    }

    if (enemy_pawns & west && west_moved_two && !this->is_white) {
        en_passant_moves |= (west >> 8);
        this->en_passant_capture_bit |= west;
    }

    if (enemy_pawns & east && east_moved_two && !this->is_white) {
        en_passant_moves |= (east >> 8);
        this->en_passant_capture_bit |= east;
    }

    return en_passant_moves;
}

void Pawn::strip_pseudo_legal_special_moves(Board& board) {

    uint8_t ep_capture_bit = BBHelper::get_first_bit(this->en_passant_capture_bit);
    uint8_t ep_move_bit = this->is_white ? ep_capture_bit + 8 : ep_capture_bit - 8;
          
    uint64_t enemy_captures = board.get_simulated_enemy_captures(this, this->bit, ep_move_bit, ep_capture_bit);

    uint64_t friendly_king = (this->is_white) ? board.bitboards[FenParser::W_KING] : board.bitboards[FenParser::B_KING];

    bool in_check = friendly_king & enemy_captures;

    // so apparently en_passant_capture_bit isnt the move bit its the capture bit, whos idea was that

    // okay keep it but call it something better i think.
    
    if (in_check && BBHelper::get_bit(this->en_passant_capture_bit, ep_capture_bit))
        this->captures = BBHelper::clear_bit(this->captures, ep_move_bit);
}