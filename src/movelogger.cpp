#pragma once

#include "movelogger.hpp"

std::vector<Move> MoveLogger::move_history;

void MoveLogger::append_move(Move move) {
    move_history.push_back(move);
    std::cout << "move appended to game_moves!" << "\n";
}