#pragma once

#include "pieces.hpp"

#include <cstdint>
#include <vector>
#include <string>

struct Move {
    
    char move_id;
    uint64_t start_bit;

    uint64_t end_bit;

    // if clicked_bit in selected_piece->captures
    // piece removed from pieces after captured so need board::readd_piece()

    bool has_capture;
    char captured_id;
    
};

class MoveLogger {
public:

    static std::vector<Move> move_history;

    static void append_move(Move move);
    static void show_algebraic_move_history();

};  