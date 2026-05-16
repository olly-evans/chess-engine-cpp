#include "util.hpp"
#include "pieces.hpp"
#include "board.hpp"
#include "bitboardhelper.hpp"
#include "movelogger.hpp"
#include "sfml_renderer.hpp"

#include <iostream>
#include <filesystem>
#include <cstdint>
#include <string>

Piece::Piece(char id, uint8_t b) : 
    id(id), 
    bit(b) {
    
    is_white = (isupper(this->id));

    // can we load each image once?
    // perhaps loaded to each piece type or smth
    // if (texture.loadFromFile(get_texture_path())) {
    //     sprite.setTexture(texture);

    //     sf::FloatRect sprite_size = sprite.getGlobalBounds();
    //     sprite.setScale(board_square_size / sprite_size.width, 
    //                     board_square_size / sprite_size.height);
    // }
}

std::string Piece::resolve_texture_path() {
    std::filesystem::path path = std::filesystem::current_path();
    std::string color_prefix = (this->is_white ? "w" : "b");
    char tmp_id = toupper(id);
    std::cout << "path for " << this->id << " is: " << path.string() + "/assets/" + color_prefix + tmp_id + ".png" + "\n";

    return path.string() + "/assets/" + color_prefix + tmp_id + ".png";
}

std::string Piece::get_texture_path() {
    return resolve_texture_path();
}

void Piece::set_bit(uint8_t bit) {
    this->bit = bit;
    // this->has_moved = true;
}

uint8_t Piece::get_bit() {
    return this->bit;
}

void Piece::strip_pseudo_legal_attacks(Board& board) {

    /* 
    *
    *  Strips piece members this->moves and this->captures of attacks that would result 
    *  in a friendly king check by simulating all of the moves. Yes, slow af but works
    *  for now.
    * 
    */

    uint64_t attacks = this->moves | this->captures;

    // Lets try and do this better.
    std::vector<uint8_t> move_bits = BBHelper::get_bit_vector(attacks);

    for (uint8_t move_bit : move_bits) {

        uint64_t enemy_captures = board.get_simulated_enemy_captures(this, this->bit, move_bit, move_bit);

        uint64_t friendly_king;

        // Make a fake king if we have a king selected, else use the real one.
        if (toupper(this->id) == 'K') {
            friendly_king = (1ULL << move_bit);
        } else {
            friendly_king = (this->is_white) ? board.bitboards[W_KING] : board.bitboards[B_KING];
        }        

        bool in_check = friendly_king & enemy_captures;

        // Strip from moves or captures any attack that would result in friendly king being in check.
        if (in_check && BBHelper::get_bit(this->moves, move_bit))
            this->moves = BBHelper::clear_bit(this->moves, move_bit);
        
        if (in_check && BBHelper::get_bit(this->captures, move_bit))
            this->captures = BBHelper::clear_bit(this->captures, move_bit);

    }
    // I don't like this, but at least its modular and in the general stripping function.
    strip_pseudo_legal_special_moves(board);
}

/* PAWN */

void Pawn::set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) {


    uint64_t pawn = 1ULL << this->bit;
    uint64_t moves = 0ULL;

    if (this->is_white) {
        moves = get_white_pawn_moves(pawn, w_bb, b_bb);

        // enpassant
        uint64_t enpassant_captures = get_enpassant(w_bb, b_bb);
        this->captures |= enpassant_captures;
        
        // promotions.
    } else {

        moves = get_black_pawn_moves(pawn , w_bb, b_bb);

        uint64_t enpassant_captures = get_enpassant(w_bb, b_bb);
        this->captures |= enpassant_captures;
    }

    moves = BBHelper::remove_friendly_pieces(moves, b_bb);
    this->moves = BBHelper::remove_enemy_pieces(moves, w_bb);
};

uint64_t Pawn::get_white_pawn_moves(uint64_t pawn, uint64_t w_bb, uint64_t b_bb) {
    
    /* White pawns as of right now will always march in the northern direction. */

    uint64_t moves = 0ULL;
    this->captures = 0ULL;

    uint64_t white_pawn_start_rank = BBHelper::rank_masks[1];

    if (b_bb & (pawn << 9)) this->captures |= ((pawn & ~BBHelper::file_masks[7]) << 9);
    if (b_bb & (pawn << 7)) this->captures |= ((pawn & ~BBHelper::file_masks[0]) << 7);

    if (w_bb & (pawn << 8) | b_bb & (pawn << 8)) return moves;
    moves |= (pawn << 8);

    // Or doesn't seem right but works so eh.
    if (!(pawn & (white_pawn_start_rank)) | (b_bb & (pawn << 16))) return moves;
    moves |= (pawn << 16);

    return moves;
}

uint64_t Pawn::get_black_pawn_moves(uint64_t pawn, uint64_t w_bb, uint64_t b_bb) {
    
    /* Black pawns as of right now will always march in the southern direction. */

    uint64_t moves = 0ULL;
    this->captures = 0ULL; // Reset so previous highlights not rendered.

    uint64_t black_pawn_start_rank = BBHelper::rank_masks[6];

    // Find the captures, mask out ones that overlap to next file.
    if (w_bb & (pawn >> 9)) this->captures |= (pawn & ~BBHelper::file_masks[0]) >> 9;
    if (w_bb & (pawn >> 7)) this->captures |= (pawn & ~BBHelper::file_masks[7]) >> 7;
    
    if (w_bb & (pawn >> 8) | b_bb & (pawn >> 8)) return moves;
    moves |= (pawn >> 8);

    // Or doesn't seem right but works so eh.
    if (!(pawn & (black_pawn_start_rank)) | (w_bb & (pawn >> 16))) return moves;
    moves |= (pawn >> 16);
    
    return moves;
}

uint64_t Pawn::get_enpassant(uint64_t w_bb, uint64_t b_bb) {

    const uint64_t no_enpassant = 0ULL;
    uint64_t en_passant_moves = 0ULL;

    uint64_t pawn = (1ULL << this->bit);

    uint64_t enemy_pawns = this->is_white ? Board::bitboards[B_PAWNS] : Board::bitboards[W_PAWNS];

    uint64_t west = (pawn << 1);
    uint64_t east = (pawn >> 1);

    if (!(enemy_pawns & west) && !(enemy_pawns & east))
        return no_enpassant;

    if (MoveLogger::move_history.empty())
        return no_enpassant;

    Move& last_move = MoveLogger::move_history.back();

    // true in our instance.
    bool moved_two = (abs(last_move.end_bit - last_move.start_bit) == 16);

    
    bool east_moved_two = moved_two && (east == (1ULL << last_move.end_bit));
    bool west_moved_two = moved_two && (west == (1ULL << last_move.end_bit));

    // this->enpassant_captures, shows the capture square
    // move to clicked bit, remove clicked_bit << 8, clicked_bit >> 8. in board.

    if (enemy_pawns & (west) && west_moved_two && this->is_white) {
        en_passant_moves |= (west << 8);
        this->en_passant_captures |= west;
    }

    if (enemy_pawns & (east) && east_moved_two && this->is_white) {
        en_passant_moves |= (east << 8);
        this->en_passant_captures |= east;
    }

    if (enemy_pawns & (west) && west_moved_two && !this->is_white) {
        en_passant_moves |= (west >> 8);
        this->en_passant_captures |= west;
    }

    if (enemy_pawns & (east) && east_moved_two && !this->is_white) {
        en_passant_moves |= (east >> 8);
        this->en_passant_captures |= east;
    }

    return en_passant_moves;
}

void Pawn::strip_pseudo_legal_special_moves(Board& board) {

    uint8_t ep_capture_bit = BBHelper::get_first_bit(this->en_passant_captures);
    uint8_t ep_move_bit = this->is_white ? ep_capture_bit + 8 : ep_capture_bit - 8;
          
    uint64_t enemy_captures = board.get_simulated_enemy_captures(this, this->bit, ep_move_bit, ep_capture_bit);

    uint64_t friendly_king = (this->is_white) ? board.bitboards[W_KING] : board.bitboards[B_KING];

    bool in_check = friendly_king & enemy_captures;

    // so apparently en_passant_captures isnt the move bit its the capture bit, whos idea was that
    if (in_check && BBHelper::get_bit(this->en_passant_captures, ep_capture_bit))
        this->captures = BBHelper::clear_bit(this->captures, ep_move_bit);
}

/* KNIGHT */

void Knight::set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) {
    
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

/* BISHOP */

void Bishop::set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) {
    uint64_t bishop = (1ULL << this->bit);
    uint64_t moves = 0ULL;

    uint64_t north_west_moves = get_north_west_moves(bishop, w_bb, b_bb);
    uint64_t north_east_moves = get_north_east_moves(bishop, w_bb, b_bb);

    uint64_t south_west_moves = get_south_west_moves(bishop, w_bb, b_bb);
    uint64_t south_east_moves = get_south_east_moves(bishop, w_bb, b_bb);

    moves |= north_west_moves;
    moves |= north_east_moves;
    moves |= south_west_moves;
    moves |= south_east_moves;

    uint64_t enemy = (this->is_white) ? b_bb : w_bb; 
    this->captures = (moves & enemy);

    moves = BBHelper::remove_friendly_pieces(moves, (this->is_white ? w_bb : b_bb)); 
    this->moves = BBHelper::remove_enemy_pieces(moves, (this->is_white) ? b_bb : w_bb);
};

uint64_t Piece::get_north_west_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_west_moves = 0ULL;
    const uint8_t north_west_offset = 9;

    uint8_t piece_file = BBHelper::get_piece_file(piece);
    uint8_t move_file;
        
    for (uint8_t i = 0; i < GRID_SZ; i++) {

        // this will be until we hit the end of the board, always 7 if no pieces in the way.
        move_file = piece_file - i;

        if (w_bb & north_west_moves) break;
        if (b_bb & north_west_moves) break;
        if (!(piece << (north_west_offset*i))) break;

         // Stop generating attacks if we get to the far left file.
        if (move_file == 0) break;

        north_west_moves |= (piece << (north_west_offset + (north_west_offset*i)));
    }
    return north_west_moves;
}

uint64_t Piece::get_north_east_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_east_moves = 0ULL;
    const uint8_t north_east_offset = 7;

    uint8_t piece_file;
    uint8_t move_file;
    
    piece_file = BBHelper::get_piece_file(piece);

    for (uint8_t i = 0; i < GRID_SZ; i++) {

        move_file = piece_file + i;

        if (w_bb & north_east_moves) break;
        if (b_bb & north_east_moves) break;
        if (!(piece << (north_east_offset*i))) break;

         // Stop generating attacks if we get to the far right file.
        if (move_file == GRID_SZ - 1) break;

        north_east_moves |= (piece << (north_east_offset + (north_east_offset*i)));
    }
    return north_east_moves;
}

uint64_t Piece::get_south_west_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_west_moves = 0ULL;
    const uint8_t north_west_offset = 7;

    uint8_t piece_file;
    uint8_t move_file;

    // Perhaps a bitboard function.
    piece_file = BBHelper::get_piece_file(piece);

    for (uint8_t i = 0; i < GRID_SZ; i++) {

        // this will be until we hit the end of the board, always 7 if no pieces in the way.
        move_file = piece_file - i;

        if (w_bb & north_west_moves) break;
        if (b_bb & north_west_moves) break;
        if (!(piece >> (north_west_offset*i))) break;

         // Stop generating attacks if we get to the far left file.
        if (move_file == 0) break;

        north_west_moves |= (piece >> (north_west_offset + (north_west_offset*i)));
    }
    return north_west_moves;
}

uint64_t Piece::get_south_east_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_east_moves = 0ULL;
    const uint8_t north_east_offset = 9;

    uint8_t piece_file;
    uint8_t move_file;
    
    piece_file = BBHelper::get_piece_file(piece);

    for (uint8_t i = 0; i < GRID_SZ; i++) {

        move_file = piece_file + i;

        if (w_bb & north_east_moves) break;
        if (b_bb & north_east_moves) break;
        if (!(piece >> (north_east_offset*i))) break;

         // Stop generating attacks if we get to the far right file.
        if (move_file == GRID_SZ - 1) break;

        north_east_moves |= (piece >> (north_east_offset + (north_east_offset*i)));
    }
    return north_east_moves;
}

void Bishop::strip_pseudo_legal_special_moves(Board& board) {
    return;
}

/* ROOK */

void Rook::set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) {

    uint64_t rook = (1ULL << this->bit);
    uint64_t moves = 0ULL;

    uint64_t north_moves = get_north_moves(rook, w_bb, b_bb);
    uint64_t south_moves = get_south_moves(rook, w_bb, b_bb);
    uint64_t west_moves = get_west_moves(rook, w_bb, b_bb);
    uint64_t east_moves = get_east_moves(rook, w_bb, b_bb);

    moves |= north_moves;
    moves |= south_moves;
    moves |= west_moves;
    moves |= east_moves;

    uint64_t enemy = (this->is_white) ? b_bb : w_bb; 
    this->captures = (moves & enemy);

    moves = BBHelper::remove_friendly_pieces(moves, (this->is_white ? w_bb : b_bb)); 
    this->moves = BBHelper::remove_enemy_pieces(moves, (this->is_white) ? b_bb : w_bb);;
};

uint64_t Piece::get_north_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t north_moves = 0ULL;
    uint8_t north_offset = 8;

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & north_moves) break;
        if (b_bb & north_moves) break;
        if (!(piece << (north_offset*i))) break;

        north_moves |= (piece << (north_offset + (north_offset*i)));

    }
    return north_moves;
}

uint64_t Piece::get_south_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t south_moves = 0ULL;
    uint8_t south_offset = 8;

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & south_moves) break;
        if (b_bb & south_moves) break;
        if (!(piece >> (south_offset*i))) break;

        south_moves |= (piece >> (south_offset + (south_offset*i)));

    }
    return south_moves;
}

uint64_t Piece::get_west_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t west_moves = 0ULL;
    uint8_t west_offset = 1;
    
    // Gets the index into rank_masks of the rank above the piece so we can mask it when shifting <<.
    uint8_t piece_bit = BBHelper::get_first_bit(piece);
    uint8_t mask_index = (piece_bit / GRID_SZ) + 1;

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & west_moves) break;
        if (b_bb & west_moves) break;
        if (!(piece << (west_offset*i))) break; // Can we continue shifting? If not then off the board.

        west_moves |= (piece << (west_offset + (west_offset*i)));
    }
    
    return (west_moves & ~(BBHelper::rank_masks[mask_index]));
}

uint64_t Piece::get_east_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb) {

    uint64_t east_moves = 0ULL;
    uint8_t east_offset = 1;
    
    // Gets the index into rank_masks of the rank above the piece so we can mask it when shifting <<.
    uint8_t piece_bit = BBHelper::get_first_bit(piece);
    uint8_t mask_index = (piece_bit / GRID_SZ) - 1; // Minus for east.

    for (uint8_t i = 0; i < GRID_SZ; i++) {
        if (w_bb & east_moves) break;
        if (b_bb & east_moves) break;
        if (!(piece >> (east_offset*i))) break; // Can we continue shifting? If not then off the board.

        east_moves |= (piece >> (east_offset + (east_offset*i)));
    }

    return (east_moves & ~(BBHelper::rank_masks[mask_index]));
}

void Rook::strip_pseudo_legal_special_moves(Board& board) {
    return;
}

/* QUEEN */

void Queen::set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) {

    uint64_t queen = (1ULL << this->bit);
    uint64_t moves = 0ULL;

    uint64_t north_moves = get_north_moves(queen, w_bb, b_bb);
    uint64_t south_moves = get_south_moves(queen, w_bb, b_bb);
    uint64_t west_moves = get_west_moves(queen, w_bb, b_bb);
    uint64_t east_moves = get_east_moves(queen, w_bb, b_bb);

    uint64_t north_west_moves = get_north_west_moves(queen, w_bb, b_bb);
    uint64_t north_east_moves = get_north_east_moves(queen, w_bb, b_bb);
    uint64_t south_west_moves = get_south_west_moves(queen, w_bb, b_bb);
    uint64_t south_east_moves = get_south_east_moves(queen, w_bb, b_bb);

    moves |= north_moves;
    moves |= south_moves;
    moves |= west_moves;
    moves |= east_moves;

    moves |= north_west_moves;
    moves |= north_east_moves;
    moves |= south_west_moves;
    moves |= south_east_moves;

    uint64_t enemy_occupancy = (this->is_white) ? b_bb : w_bb; 
    this->captures = (moves & enemy_occupancy);
    
    moves = BBHelper::remove_friendly_pieces(moves, (this->is_white ? w_bb : b_bb)); 
    this->moves = BBHelper::remove_enemy_pieces(moves, (this->is_white) ? b_bb : w_bb);
};

void Queen::strip_pseudo_legal_special_moves(Board& board) {
    return;
}

/* KING */

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

    uint8_t kings_file = BBHelper::get_piece_file(king);
    if (kings_file == 7) moves &= ~BBHelper::file_masks[kings_file];
    if (kings_file == 0) moves &= ~BBHelper::file_masks[kings_file];

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
    uint64_t rooks = (this->is_white) ? Board::bitboards[W_ROOKS] : Board::bitboards[B_ROOKS];

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

