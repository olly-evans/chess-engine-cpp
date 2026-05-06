
#include <optional>
#include <iostream>

#include "pieces.hpp"

class FenParser {
public:

    static std::vector<std::string> split(const std::string& str);
    static std::vector<std::string> split_with_delimiter(std::string s, const std::string& delimiter);

    static uint64_t& get_fen_char_bitboard(char ch, std::vector<uint64_t>& bitboards);
};