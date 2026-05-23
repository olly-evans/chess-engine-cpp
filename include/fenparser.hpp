#pragma once

#include <optional>
#include <iostream>
#include <array>

#include "pieces.hpp"

constexpr uint8_t NUM_PIECE_TYPES = 12; /* White rook, black pawn etc.. */

class FenParser {
public:

    FenParser();

    enum Bitboards { W_PAWNS, W_KNIGHTS, W_BISHOPS, W_ROOKS, W_QUEEN, W_KING,
                 B_PAWNS, B_KNIGHTS, B_BISHOPS, B_ROOKS, B_QUEEN, B_KING 
    };

    std::vector<std::string> split(const std::string& str);
    std::vector<std::string> split_with_delimiter(std::string s, const std::string& delimiter);

    uint64_t& get_fen_char_bitboard(char ch, std::array<uint64_t, NUM_PIECE_TYPES>& bitboards);
    char get_bitboards_fen_char(uint8_t index);

    /* PARSING */

    void parse_fen_position(Board& board, std::string fen_pos_sub_str);
    void parse_and_set_fen_enpassant(Board& board, std::string ep_target);


};