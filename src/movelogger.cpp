#pragma once

#include "movelogger.hpp"

std::vector<Move> MoveLogger::move_history;

void MoveLogger::append_move(Move move) {
    move_history.push_back(move);
    std::cout << "move appended to game_moves!" << "\n";
}

void MoveLogger::show_moves()  {
    
        for (int i = 0; i < move_history.size(); i++) {
            std::cout << "Move " << i << ": " << move_history[i].m_start_bit << ", " << move_history[i].m_end_bit << ", " << move_history[i].captured_piece << "\n";
        }
}