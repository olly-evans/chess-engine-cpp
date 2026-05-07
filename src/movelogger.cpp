
#include "movelogger.hpp"
#include "bitboardhelper.hpp"
#include "board.hpp"

std::vector<Move> MoveLogger::move_history;

void MoveLogger::show_algebraic_move_history() {

    if (move_history.size() < 1) return;

    std::cout << "Updated Move History:\n";

    // std::cout << "\033[2J\033[1;1H";

    for (int i = 0; i < move_history.size(); i++) {
        
        uint64_t selected_piece = 0ULL;
        BBHelper::set_bit(selected_piece, move_history[i].start_bit);
        uint8_t selected_piece_file = BBHelper::get_piece_file(selected_piece);


        // TODO: if we have two kinds of this piece on the same rank/file use the other.

        // selected_piece_rank = 0,1,2,3 -> a,b,c,d whatever.
        std::string move_square = BBHelper::bit_to_square_name[move_history[i].end_bit];

        char capture = 'x';

        if (move_history[i].has_capture) {
            std::cout << move_history[i].move_id << capture << move_square << "\n";
        } else {
            std::cout << move_history[i].move_id << move_square << "\n";
        }
    }
}

void MoveLogger::log_move(std::vector<uint64_t> bitboards, std::vector<char> bb_names, uint8_t clicked_bit, uint8_t moved_bit, char moved_id, bool has_capture) {
    // Piece* captured_piece = get_piece(clicked_bit);
    // bool has_capture = (captured_piece) ? true : false;
    // char capture_id = (has_capture) ? captured_piece->piece_id : '\0';
    
    uint8_t i;
    for (i = 0; i < bb_names.size(); i++) {
        if (bitboards[i] & (1ULL << clicked_bit)) break;
    }
    char captured_id = bb_names[i];

    // Fill out move data.
    Move move = {moved_id, 
                 moved_bit, 
                 clicked_bit,
                 has_capture, 
                 captured_id};

    MoveLogger::move_history.push_back(move);
}

// bool MoveLogger::check_draw_by_threefold_repetition() {

// }