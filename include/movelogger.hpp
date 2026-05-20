#pragma once

#include "pieces.hpp"
#include "board.hpp"

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

    static std::vector<Move> move_history; /* Vector of move structs containing the moves played throughout the game. */

public:
    static void show_algebraic_move_history();
    static void show_algebraic_moves();
    static void move_to_algebraic_notation(Move move);

    static void log_move(Board& board,
                          uint8_t clicked_bit, 
                          uint8_t moved_bit, 
                          char moved_id
                        );
};  