#pragma once

#include "pieces.hpp"

#include <cstdint>
#include <vector>
#include <string>

struct Move {

    uint64_t m_start_bit;
    uint64_t m_end_bit;

    // if clicked_bit in selected_piece->captures
    // piece removed from pieces after captured so need board::readd_piece()

    std::string captured_id;
    Color captured_col;
};

class MoveLogger {
public:

    static std::vector<Move> move_history;

    // not necessary i think but get the idea.
    // lets add the moves for one piece and print them.
    static void append_move(Move move);

    // void show_moves() {
    //     for (auto& move : game_moves) {
    //         std::cout << move << "\n";
    //     }
    // }
};  