
#include "movelogger.hpp"
#include "bitboardhelper.hpp"
#include "board.hpp"

std::vector<Move> MoveLogger::move_history;

void MoveLogger::show_algebraic_move_history() {

    if (move_history.empty()) 
        return;

    for (int i = 0; i < move_history.size(); i++) {
        move_to_algebraic_notation(move_history[i]);
    }
}

void MoveLogger::show_algebraic_moves() {
    if (move_history.empty())
        return;

    Move& last_move = move_history.back();
    move_to_algebraic_notation(last_move);

}

void MoveLogger::move_to_algebraic_notation(Move move) {

    uint64_t selected_piece = 0ULL;
    BBHelper::set_bit(selected_piece, move.start_bit);
    uint8_t selected_piece_file = BBHelper::get_piece_file(selected_piece);

    // TODO: if we have two kinds of this piece on the same rank/file use the other.

    // selected_piece_rank = 0,1,2,3 -> a,b,c,d whatever.
    std::string move_square = BBHelper::bit_to_square_name[move.end_bit];

    char capture = 'x';

    if (move.has_capture) {
        std::cout << move.moved_id << capture << move_square << "\n";
    } else {
        std::cout << move.moved_id << move_square << "\n";
    }
}

void MoveLogger::log_move(Board& board,
                          uint8_t clicked_bit, 
                          uint8_t moved_bit, 
                          char moved_id
                          ) {
    
    /* Logs move and calculates has_capture, captured_id and capture_bit for Move struct */

    uint8_t capture_bit;
    uint8_t ep_capture_bit = (isupper(moved_id)) ? clicked_bit - 8 : clicked_bit + 8;

    bool is_ep_capture = board.is_enpassant_capture(clicked_bit);
    bool has_capture   = board.bit_has_piece(clicked_bit) || is_ep_capture;

    capture_bit = (!is_ep_capture) ? clicked_bit : ep_capture_bit;

    // find the bitboards char to set id of captured piece.

    uint8_t i;
    for (i = 0; i < board.bitboard_names.size(); i++) {
        if (board.bitboards[i] & (1ULL << capture_bit))
            break;
    }
    char captured_id = board.bitboard_names[i];

    // Fill out move data.
    Move move = {moved_id, 
                 moved_bit, 
                 clicked_bit,
                 has_capture, 
                 captured_id,
                 capture_bit};

    MoveLogger::move_history.push_back(move);
}

// bool MoveLogger::check_draw_by_threefold_repetition() {

// }