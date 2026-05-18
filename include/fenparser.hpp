
#include <optional>
#include <iostream>

#include "pieces.hpp"

class FenParser {
public:

    enum Bitboards { W_PAWNS, W_KNIGHTS, W_BISHOPS, W_ROOKS, W_QUEEN, W_KING,
                 B_PAWNS, B_KNIGHTS, B_BISHOPS, B_ROOKS, B_QUEEN, B_KING 
    };

    static std::vector<std::string> split(const std::string& str);
    static std::vector<std::string> split_with_delimiter(std::string s, const std::string& delimiter);

    static uint64_t& get_fen_char_bitboard(char ch, std::vector<uint64_t>& bitboards);
    static char get_bitboards_fen_char(uint8_t index);
};