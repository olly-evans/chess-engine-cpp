#pragma once

#include "pieces.hpp"

#include <cstdint>
#include <vector>
#include <string>

struct Move {
    
    char piece_id;
    uint64_t start_bit;
    Color color;

    uint64_t end_bit;

    // if clicked_bit in selected_piece->captures
    // piece removed from pieces after captured so need board::readd_piece()
    Piece* captured_piece;
};

class MoveLogger {
public:

    static std::vector<Move> move_history;

    static void append_move(Move move);
    static void show_algebraic_move_history();
};  