#include "SFML/Graphics.hpp"

#include "piece.hpp"
#include "debug.hpp"
#include "bitboardhelper.hpp"
#include "player.hpp"
#include "board.hpp"
#include "movelogger.hpp"

#include "pieces/pawn.hpp"
#include "pieces/knight.hpp"
#include "pieces/bishop.hpp"
#include "pieces/rook.hpp"
#include "pieces/queen.hpp"
#include "pieces/king.hpp"

#include <sstream>
#include <iostream>
#include <cmath>


std::array<uint64_t, NUM_PIECE_TYPES> Board::bitboards;

Board::Board(std::string fen) : fen(fen), fen_parser()
{    
    selected_piece = nullptr;
    castling_rights = 0;

    bitboards = {
        w_pawns, w_knights, w_bishops, w_rooks, w_queen, w_king,
        b_pawns, b_knights, b_bishops, b_rooks, b_queen, b_king
    };

    bitboard_names = {
        'P', 'N', 'B', 'R', 'Q', 'K',
        'p', 'n', 'b', 'r', 'q', 'k',
    };
};

Board::~Board() {}

/* INIT */

void Board::init() {
    pieces.reserve(NUM_PIECE_TYPES*3); /* Means vector should remain in contiguous region and not be reallocated. */

    // Init map of square names to bits.
    BBHelper::init_name_to_bit();
    load_position_from_fen(fen);

}

void Board::load_position_from_fen(std::string fen) {

    /* Parses fen string to appropriately initialise bitboards and create needed pieces. */

    // Am not expecting malicious fen string input as of right now, will handle it when i can be bothered.


    enum FenTokens {
        PIECE_PLACEMENT = 0,
        TURN,
        CASTLING,
        ENPASSANT_SQUARE,
        HALF_MOVE_CLOCK,
        FULL_MOVE_NUM
    };

    // Make this an array jesus.
    std::vector<std::string> fen_tokens = fen_parser.split_with_delimiter(fen, " ");

    fen_parser.parse_fen_position(*this, fen_tokens[PIECE_PLACEMENT]);

    is_whites_turn = (fen_tokens[TURN] == "w");

    fen_parser.parse_and_set_fen_castling_rights(*this, fen_tokens[CASTLING]);

    fen_parser.parse_and_set_fen_enpassant(*this, fen_tokens[ENPASSANT_SQUARE]);

    // Half-move clock.
    // Full-move clock.

}

uint8_t Board::get_castling_rights() {
    return castling_rights;
}
void Board::set_castling_rights_bit(uint8_t bit) {
    castling_rights |= (1 << bit);
}

/* BITBOARD METHODS */

uint64_t Board::white_occupancy() {
    return bitboards[fen_parser.W_PAWNS] | bitboards[fen_parser.W_KNIGHTS] | bitboards[fen_parser.W_BISHOPS] |
           bitboards[fen_parser.W_ROOKS] | bitboards[fen_parser.W_QUEEN]   | bitboards[fen_parser.W_KING];
}

uint64_t Board::black_occupancy() {
    return bitboards[fen_parser.B_PAWNS] | bitboards[fen_parser.B_KNIGHTS] | bitboards[fen_parser.B_BISHOPS] |
           bitboards[fen_parser.B_ROOKS] | bitboards[fen_parser.B_QUEEN]   | bitboards[fen_parser.B_KING];
}

uint64_t Board::get_white_captures(uint64_t white, uint64_t black) {

    uint64_t white_captures = 0ULL;

    for (auto& piece : pieces) {

            if (!piece->is_white)
                continue;
            
            // This just means if we pass in a pseudo white/black occupancy we can ignore certain captures.
            if (!(white & (1ULL << piece->bit)))
                continue;

            piece->set_pseudo_legal_attacks(white, black, castling_rights);

            white_captures |= piece->captures;
        }
    return white_captures;
}

uint64_t Board::get_black_captures(uint64_t white, uint64_t black) {

    uint64_t black_captures = 0ULL;
    for (auto& piece : pieces) {

            if (piece->is_white)
                continue;
            
            // This just means if we pass a pseudo white/black occupancy we can ignore certain captures.
            if (!(black & (1ULL << piece->bit)))
                continue;

            piece->set_pseudo_legal_attacks(white, black, castling_rights);

            black_captures |= piece->captures;
        }
    return black_captures;
}

uint64_t Board::get_simulated_enemy_captures(Piece* piece, uint8_t start, uint8_t end, uint8_t capture) {

    /* Take a proposed move, simulate it with temporary bitboards and 
    *  return the enemy colors captures. 
    *
    *  Note: Even for one move its expensive as must update all color moves/captures.
    * 
    */
    
    uint64_t white_occ;
    uint64_t black_occ;
    uint64_t enemy_captures;
    
    if (piece->is_white) {
        // Set fake occupancy bitboard with proposed move.
        white_occ = BBHelper::set_bit(white_occupancy(), end);
        white_occ = BBHelper::clear_bit(white_occ, piece->bit);

        // Remove move from enemy occupancy bitboard incase our fake move is a capture.
        black_occ = BBHelper::clear_bit(black_occupancy(), capture);

        // Get enemy_captures with our fake occupancy bitboards.
        enemy_captures = get_black_captures(white_occ, black_occ);

    } else {
        black_occ = BBHelper::set_bit(black_occupancy(), end);
        black_occ = BBHelper::clear_bit(black_occ, piece->bit);

        white_occ = BBHelper::clear_bit(white_occupancy(), capture);

        enemy_captures = get_white_captures(white_occ, black_occ);
    }  
    
    return enemy_captures;
}

// // Don't use this right now but could be useful.
// void Board::update_all_piece_attacks() {

//     for (auto& piece : pieces) {
//         piece->set_pseudo_legal_attacks(white_occupancy(), black_occupancy());
//         piece->strip_pseudo_legal_attacks(*this);
//     }
// }

void Board::create_piece(const char id, uint8_t bit) {

    // board may need texture_cache
    switch (toupper(id)) {

        case 'P':
            pieces.emplace_back(new Pawn(id, bit));
            break;
        case 'N':
            pieces.emplace_back(new Knight(id, bit));
            break;
        case 'B':
            pieces.emplace_back(new Bishop(id, bit));
            break;
        case 'R':
            pieces.emplace_back(new Rook(id, bit));
            break;
        case 'Q':
            pieces.emplace_back(new Queen(id, bit));
            break;
        case 'K':
            pieces.emplace_back(new King(id, bit));
            break;
        default: 
            break;
    }
}

void Board::undo_move() {

    if (MoveLogger::move_history.empty()) 
        return;

    Move& last_move = MoveLogger::move_history.back();

    // Just so if we undo a move whilst user has piece selected we don't get funny business.
    // if (selected_piece) 
    //     reset_move_and_capture_highlights(selected_piece.bit);

    uint64_t& moved_piece_bitboard = fen_parser.get_fen_char_bitboard(last_move.moved_id, bitboards);

    BBHelper::set_bit_by_ref(moved_piece_bitboard, last_move.start_bit);
    BBHelper::clear_bit_by_ref(moved_piece_bitboard, last_move.end_bit);
    
    std::shared_ptr<Piece> moved_piece = get_piece(last_move.end_bit);
    moved_piece->set_bit(last_move.start_bit);
    
    // If the move didn't involve a capture we can clean up and return early.
    if (!last_move.has_capture) {
        MoveLogger::move_history.pop_back(); 
        return;
    }

    // capture_bit changes depending on move type in handle_piece_move.
    create_piece(last_move.captured_id, last_move.capture_bit);
    uint64_t& captured_piece_bitboard = fen_parser.get_fen_char_bitboard(last_move.captured_id, bitboards);
    BBHelper::set_bit_by_ref(captured_piece_bitboard, last_move.capture_bit);

    MoveLogger::move_history.pop_back();
}

/* PIECE FUNCTIONALITY */

std::shared_ptr<Piece> Board::select_piece(uint8_t clicked_bit) {

    // perhaps just make a an event_handler function. does make sense.
    // does need squares though

    // eventually don't use a pointer.
    std::shared_ptr<Piece> piece = get_piece(clicked_bit);
    if (!piece) 
        return nullptr;
    
    if (!piece->is_white && is_whites_turn) 
        return nullptr;

    if (piece->is_white && !is_whites_turn) 
        return nullptr;

    piece->set_pseudo_legal_attacks(white_occupancy(), black_occupancy(), castling_rights); 
    piece->strip_pseudo_legal_attacks(*this); // Essentially adds check checks.

    return piece;
}

std::shared_ptr<Piece> Board::get_piece(uint8_t clicked_bit) {

    for (auto& piece : pieces) {
        if (!(clicked_bit == piece->bit))
            continue;

        std::shared_ptr<Piece> tmp = piece;
        return tmp;
    }
    return nullptr;
}

bool Board::bit_has_piece(uint8_t clicked_bit) {
    
    for (auto& bitboard : bitboards) {
        if (bitboard & (1ULL << clicked_bit)) 
            return true;
    }
    return false;
}

void Board::remove_piece(uint8_t piece_to_remove_bit) {

    auto it = std::find_if(pieces.begin(), pieces.end(), [piece_to_remove_bit](const std::shared_ptr<Piece>& p) {
        return p->bit == piece_to_remove_bit;
    });

    if (it != pieces.end()) {
        pieces.erase(it);  // remove from vector
    }
}

void Board::make_move(Move move) {

    if (move.has_capture) {
        uint64_t& captured = fen_parser.get_fen_char_bitboard(move.captured_id, bitboards);
        BBHelper::clear_bit_by_ref(captured, move.capture_bit);
        remove_piece(move.capture_bit);
    }

    if (move.is_castle) {
        if (isupper(move.moved_id)) {            
            handle_white_castle_move(move.end_bit);
        } else {
            handle_black_castle_move(move.end_bit);
        }
    }

    // 1111 & 1101, castling_rights & w_kingside_rook moved
    castling_rights &= c_rights[move.start_bit]; // can surely do the same with enpassant.

    uint64_t& moved = fen_parser.get_fen_char_bitboard(move.moved_id, bitboards);
    BBHelper::clear_bit_by_ref(moved, move.start_bit);
    BBHelper::set_bit_by_ref(moved, move.end_bit);

    selected_piece->set_bit(move.end_bit);
}

void Board::handle_white_castle_move(uint64_t end_bit) {
    
    bool queenside = (BBHelper::square_name_to_bit("c1") == end_bit);
    bool kingside = (BBHelper::square_name_to_bit("g1") == end_bit);

    uint8_t rook_bit;
    uint8_t rook_move_bit;
    if (queenside) {
        rook_bit = BBHelper::square_name_to_bit("a1");
        rook_move_bit = BBHelper::square_name_to_bit("d1");
    }

    if (kingside) {
        rook_bit = BBHelper::square_name_to_bit("h1");
        rook_move_bit = BBHelper::square_name_to_bit("f1");
    }          

    std::shared_ptr<Piece> p = get_piece(rook_bit);
    p->set_bit(rook_move_bit);

    uint64_t& white_rooks = fen_parser.get_fen_char_bitboard('R', bitboards);
    BBHelper::clear_bit_by_ref(white_rooks, rook_bit);
    BBHelper::set_bit_by_ref(white_rooks, rook_move_bit);
}

void Board::handle_black_castle_move(uint64_t end_bit) {

    bool queenside = (BBHelper::square_name_to_bit("c8") == end_bit);
    bool kingside = (BBHelper::square_name_to_bit("g8") == end_bit);

    uint8_t rook_bit;
    uint8_t rook_move_bit;

    if (queenside) {
        rook_bit = BBHelper::square_name_to_bit("a8");
        rook_move_bit = BBHelper::square_name_to_bit("d8");
    }

    if (kingside) {
        rook_bit = BBHelper::square_name_to_bit("h8");
        rook_move_bit = BBHelper::square_name_to_bit("f8");
    }
    
    std::shared_ptr<Piece> p = get_piece(rook_bit);
    p->set_bit(rook_move_bit);

    uint64_t& black_rooks = fen_parser.get_fen_char_bitboard('r', bitboards);
    BBHelper::clear_bit_by_ref(black_rooks, rook_bit);
    BBHelper::set_bit_by_ref(black_rooks, rook_move_bit);

}

bool Board::is_enpassant_capture(uint8_t clicked_bit) {

    std::shared_ptr<Pawn> pawn = std::dynamic_pointer_cast<Pawn>(selected_piece);

    if (!pawn) 
        return false;

    bool piece_attacked = (pawn->is_white) ? (black_occupancy() & (1ULL << clicked_bit)) 
                                           : (white_occupancy() & (1ULL << clicked_bit));

    if (piece_attacked)
        return false;

    uint8_t color_ep_offset = pawn->is_white ? -8 : 8;
    uint8_t ep_capture_bit = clicked_bit + color_ep_offset;
    bool enemy_on_ep_bit = (pawn->is_white) ? (black_occupancy() & (1ULL << ep_capture_bit)) 
                                            : (white_occupancy() & (1ULL << ep_capture_bit));

    if (!enemy_on_ep_bit)
        return false;

    // this is the only place we use en_passant_capture_bit...
    // could just have this is == 0ULL or smth.
    if (!(pawn->en_passant_capture_bit & (1ULL << (ep_capture_bit))))
        return false;

    // /* 
    //    Get to here and: we have a pawn selected, there is no enemy piece on clicked_bit, 
    //    there is an enemy piece on ep_capture_bit and ep_capture_bit is a 
    //    valid en passant capture.
    // */

    return true;  
}