#pragma once

#include "pieces.hpp"

#include <cstdint>
#include <vector>
#include <string>
#include <functional>

struct Move {
    
    char moved_id;

    uint64_t start_bit;
    uint64_t end_bit;


    bool has_capture;
    char captured_id;
    uint8_t capture_bit;
    
};

class MoveLogger {
public:

    static std::vector<Move> move_history;

    static void append_move(Move move);
    static void show_algebraic_move_history();

    static void log_move(std::vector<uint64_t> bitboards, 
                         std::vector<char> bb_names, 
                         uint8_t clicked_bit, uint8_t moved_bit, 
                         char moved_id, 
                         bool has_capture,
                         uint8_t capture_bit);
};  