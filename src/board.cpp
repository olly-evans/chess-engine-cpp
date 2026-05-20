#include "SFML/Graphics.hpp"

#include "pieces.hpp"
#include "debug.hpp"
#include "bitboardhelper.hpp"
#include "player.hpp"
#include "board.hpp"
#include "gamestate.hpp"
#include "movelogger.hpp"

#include <sstream>
#include <iostream>
#include <cmath>

// class AbstractBoard {
//     Board::Board() {}
//     .
//     .
//     .
// };

std::vector<uint64_t> Board::bitboards;

Board::Board() {

    bitboards = {
        w_pawns, w_knights, w_bishops, w_rooks, w_queen, w_king,
        b_pawns, b_knights, b_bishops, b_rooks, b_queen, b_king
    };

    bitboard_names = {
        'P', 'N', 'B', 'R', 'Q', 'K',
        'p', 'n', 'b', 'r', 'q', 'k',
    };
};

/* INIT */

void Board::init() {

    pieces.reserve(32); /* Means vector should remain in contiguous region and not be completely reallocated. */

    // Init map of square names to bits.
    BBHelper::init_name_to_bit();
   
    // I like this for now. Keeps it in init and only runs if debug enabled.
    // if (Debug::enabled) Board::init_bitboard_window_squares();

    // What needs to happen if fen string is invalid.


    // pass into board
    // good check test: "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
    // std::string ep_discovered_check = "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1";
    // std::string fen = "8/8/8/4k3/8/4P3/4K3/8 w - - 0 1";
    std::string fen = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
    // std::string fen = "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1";
    load_position_from_fen(fen);
}

// void Board::init_players() {

//     bool is_white = true; //temp

//     white_player = new Human(is_white);
//     black_player = new Engine(!is_white);

// }

void Board::load_position_from_fen(std::string fen) {

    /* Parses fen string and appropriately initialises bitboards. */

    std::vector<std::string> fen_tokens = FenParser::split_with_delimiter(fen, " ");

    // I want these hardcoded tokens to have seperate functions.
    std::string position = fen_tokens[0];

    // parse_fen_position();
    uint8_t rank = 7, file = 0;
    for (char ch : position) {

        if (ch == '/') {
            rank--;
            file = 0;
        } else if (isdigit(ch)) {
            file += ch - '0';
        } else if (isalpha(ch)) {
            uint8_t bit = rank * 8 + (7 - file);

            create_piece(ch, bit);
            file++;

            // Get correct piece type bitboard from ch.
            uint64_t& bitboard = FenParser::get_fen_char_bitboard(ch, bitboards);
            BBHelper::set_bit_by_ref(bitboard, bit);
        }
    }

    is_whites_turn = (fen_tokens[1] == "w");

    // parse_fen_enp();
    // need to let the correct pawn know that this is now available.
    std::string en_passant_target = fen_tokens[3];

    if (en_passant_target == "-") 
        return;
    
    uint8_t bit = BBHelper::square_name_to_bit(en_passant_target);
    
    uint64_t en_passant_bit = 1ULL << bit;

    // get pawn that can capture here.
    // append bit to its en_passant_capture_bit.

    // Parse more tokens later if we want to.
}

/* BITBOARD METHODS */

uint64_t Board::white_occupancy() {
    return bitboards[FenParser::Bitboards::W_PAWNS] | bitboards[FenParser::Bitboards::W_KNIGHTS] | bitboards[FenParser::Bitboards::W_BISHOPS] |
           bitboards[FenParser::Bitboards::W_ROOKS] | bitboards[FenParser::Bitboards::W_QUEEN]   | bitboards[FenParser::Bitboards::W_KING];
}

uint64_t Board::black_occupancy() {
    return bitboards[FenParser::Bitboards::B_PAWNS] | bitboards[FenParser::Bitboards::B_KNIGHTS] | bitboards[FenParser::Bitboards::B_BISHOPS] |
           bitboards[FenParser::Bitboards::B_ROOKS] | bitboards[FenParser::Bitboards::B_QUEEN]   | bitboards[FenParser::Bitboards::B_KING];
}

uint64_t Board::get_white_captures(uint64_t white, uint64_t black) {

    uint64_t white_captures = 0ULL;
    for (auto& piece : pieces) {

            if (!piece->is_white)
                continue;
            
            // This just means if we pass in a pseudo white/black occupancy we can ignore certain captures.
            if (!(white & (1ULL << piece->bit)))
                continue;

            piece->set_pseudo_legal_attacks(white, black);

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

            piece->set_pseudo_legal_attacks(white, black);

            black_captures |= piece->captures;
        }
    return black_captures;
}

uint64_t Board::get_simulated_enemy_captures(Piece* piece, uint8_t start, uint8_t end, uint8_t capture) {

    /* take in a proposed move, simulate it with temporary bitboards and 
    *  return the enemy colors captures. 
    *  Note: Even for one move its expensive as must update all color moves/captures.
    */
    
    uint64_t white_occ;
    uint64_t black_occ;
    uint64_t enemy_captures;
    
    if (piece->is_white) {
        // Make fake bitboard with proposed move.
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

bool Board::white_king_in_check(uint64_t white, uint64_t black) {

    uint64_t king = bitboards[FenParser::Bitboards::W_KING];
    uint64_t enemy_captures = get_black_captures(white, black);
    if (enemy_captures & king)
        return true;

    return false;
}

// also dont use rn, but eh
bool Board::black_king_in_check(uint64_t white, uint64_t black) {

    uint64_t king = bitboards[FenParser::Bitboards::B_KING];
    uint64_t enemy_captures = get_white_captures(white, black);

    if (enemy_captures & king)
        return true;
    return false;
}

// Don't use this right now but could be useful.
void Board::update_all_piece_attacks() {

    for (auto& piece : pieces) {
        piece->set_pseudo_legal_attacks(white_occupancy(), black_occupancy());
        piece->strip_pseudo_legal_attacks(*this);
    }
}

void Board::create_piece(const char id, uint8_t bit) {

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
        default: break;
    }
}



// void init_board_coords() {
//     // only need to place numbers vert and letters hor.

//     // if % 8 == 0, then its a number.

//     for (int i = 0; i < GRID_NUM_SQUARES; i++) {
//         if ()
//     }
// }


void Board::undo_move() {

    if (MoveLogger::move_history.empty()) 
        return;

    Move& last_move = MoveLogger::move_history.back();

    // Just so if we undo a move whilst user has piece selected we don't get funny business.
    // if (selected_piece) 
    //     reset_move_and_capture_highlights(selected_piece->bit);

    uint64_t& moved_piece_bitboard = FenParser::get_fen_char_bitboard(last_move.moved_id, bitboards);

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
    uint64_t& captured_piece_bitboard = FenParser::get_fen_char_bitboard(last_move.captured_id, bitboards);
    BBHelper::set_bit_by_ref(captured_piece_bitboard, last_move.capture_bit);

    MoveLogger::move_history.pop_back();
}

/* PIECE FUNCTIONALITY */

std::shared_ptr<Piece> Board::select_piece(uint8_t clicked_bit) {

    // perhaps just make a an event_handler function. does make sense.
    // does need squares though
    std::shared_ptr<Piece> piece = get_piece(clicked_bit);
    if (!piece) 
        return nullptr;

    // TODO:
    // Works fine but will eventually be a dependancy nightmare, perhaps move checks to gamestate class.
    // bool can_white_movcreatee()
    // bool can_black_move()
    
    if (!piece->is_white && is_whites_turn) 
        return nullptr;

    if (piece->is_white && !is_whites_turn) 
        return nullptr;

    // piece->moves and captures set.
    piece->set_pseudo_legal_attacks(white_occupancy(), black_occupancy());
    piece->strip_pseudo_legal_attacks(*this);
    
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

void Board::handle_piece_move(uint8_t clicked_bit) {

    /*

        This function is a bit funny. For it to be called selected_piece
        must not be null so no check needed.

        Essentially we find the bitboard of the selected piece type and update
        it.

        Find the bitboard of the clicked bit if any, clear the bitboard bit and
        place the selected_piece there by updating its piece->bit.

        If a piece is selected we cannot attack/move to a friendly piece, so no
        need for that logic.

    */

    /* Log Move */

    // prob dont need selected bit and that if passing in whole board.
    MoveLogger::log_move(*this, clicked_bit, selected_piece->bit, selected_piece->id);

    /* Process clicked_bit into a move */
    
    // White moving up, black moves down. Capture bit for EP is clicked_bit +-8 bits depending on color.
    uint8_t ep_capture_bit = (selected_piece->is_white) ? clicked_bit - 8 : clicked_bit + 8;
    bool is_ep_capture = is_enpassant_capture(clicked_bit);

    // handle_enpassant_move();
    // handle_move();
    // split it up.

        // yeah i want this split up .
    for (auto& bitboard: bitboards) {

        // goes first because otherwise we move the piece before checking if enpassant.

        if (is_ep_capture && BBHelper::get_bit(bitboard, ep_capture_bit)) {

            // Enpassant capture.
            bitboard = BBHelper::clear_bit(bitboard, ep_capture_bit);
            remove_piece(ep_capture_bit);
        } else if (BBHelper::get_bit(bitboard, selected_piece->bit)){

            // No capture, just a move.
            bitboard = BBHelper::clear_bit(bitboard, selected_piece->bit);
            bitboard = BBHelper::set_bit(bitboard, clicked_bit);
        } else if (BBHelper::get_bit(bitboard, clicked_bit)) {

            // Normal capture.
            bitboard = BBHelper::clear_bit(bitboard, clicked_bit);
            remove_piece(clicked_bit);
        } 
    }

    selected_piece->set_bit(clicked_bit); 
}

bool Board::is_enpassant_capture(uint8_t clicked_bit) {

    std::shared_ptr<Pawn> pawn = std::dynamic_pointer_cast<Pawn>(selected_piece);
    
    if (!pawn) 
        return false;

    // can do this in one loop and pass both bits.
    if (bit_has_piece(clicked_bit))
        return false;

    uint8_t color_ep_offset = pawn->is_white ? -8 : 8;
    uint8_t ep_capture_bit = clicked_bit + color_ep_offset;

    if (!bit_has_piece(ep_capture_bit))
        return false;

    // this is the only place we use en_passant_capture_bit...
    // could just have this is == 0ULL or smth.
    if (!(pawn->en_passant_capture_bit & (1ULL << (ep_capture_bit))))
        return false;

    /* 
       Get to here and: we have a pawn selected, there is no piece on clicked_bit, 
       there is a piece on ep_capture_bit and ep_capture_bit is a 
       valid en passant capture.
    */

    return true;  
}