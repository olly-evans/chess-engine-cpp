#pragma once

#include "movelogger.hpp"
#include "bitboardhelper.hpp"
#include "board.hpp"

#include <functional>

std::vector<Move> MoveLogger::move_history;


void MoveLogger::show_algebraic_move_history()  {

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