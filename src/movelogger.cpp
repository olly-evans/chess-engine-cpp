#pragma once

#include "movelogger.hpp"
#include "bitboardhelper.hpp"

std::vector<Move> MoveLogger::move_history;


void MoveLogger::show_algebraic_move_history()  {

    std::cout << "New Move History: \n";
    for (int i = 0; i < move_history.size(); i++) {
        
        uint64_t selected_piece = 0ULL;
        BBHelper::set_bit(selected_piece, move_history[i].m_start_bit);
        uint8_t selected_piece_file = BBHelper::get_piece_file(selected_piece);

        char capture = 'x';
        
        if (move_history[i].captured_piece) {
            std::cout << move_history[i].piece_id << capture << "square" << "\n";
        } else {
            std::cout << move_history[i].piece_id << "square" << "\n";
        }


    }
}