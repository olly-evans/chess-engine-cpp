
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

    /* prints moves to terminal move by move. */
    
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

void MoveLogger::log_move(Move move) {    
    MoveLogger::move_history.push_back(move);
}

Move MoveLogger::format_move(Board& board, uint8_t clicked_bit) {

    char moved_id = board.selected_piece->id;
    uint8_t moved_bit = board.selected_piece->bit;

    uint8_t capture_bit = -1; // Off the board.
    uint8_t ep_capture_bit = (isupper(moved_id)) ? clicked_bit - 8 : clicked_bit + 8;

    bool has_capture = false;

    // can we compare clicked_bit to capture bit for enpassant check.
    // at this point flow we dont know the capture bit for certain.

    // can we decipher capture bit before this check, becuase then it becomes very simple.
    bool is_ep_capture = board.is_enpassant_capture(clicked_bit);
    has_capture = board.bit_has_piece(clicked_bit) || is_ep_capture;

    capture_bit = (!is_ep_capture) ? clicked_bit : ep_capture_bit;

    // check for a castle.
    // has_capture false by default so handled.
    bool is_castle = (toupper(moved_id) == 'K' && (abs(moved_bit - clicked_bit) > 1)) ? true : false;

    
    std::cout << "is_castle: " << is_castle << "\n";


    // find the bitboards char to set id of captured piece.
    
    // i want to get the captured_id.
    // we have a capture_bit.

    // convert bit to id.
    // in other words bit to index in bitboards.

    // or all bitboards then check if BBHelper

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
                 capture_bit,
                 is_castle
                };
    return move;
}

// bool MoveLogger::check_draw_by_threefold_repetition() {

// }