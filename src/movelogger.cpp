#pragma once

#include "movelogger.hpp"

std::vector<Move> MoveLogger::move_history;


void MoveLogger::show_moves()  {

    std::cout << "New Move History: \n";
    for (int i = 0; i < move_history.size(); i++) {
        
        // std::cout << move_history[i].piece_id
    }
}