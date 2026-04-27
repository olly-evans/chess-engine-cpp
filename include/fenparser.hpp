
#include <optional>
#include <iostream>

#include "pieces.hpp"

struct PieceInfo {
    Color color;
    char piece_id;
};

class FenParser {
public:
    static PieceInfo get_fen_char_info(const char ch);
    static std::vector<std::string> split(const std::string& str);
    static uint64_t& get_fen_char_bitboard(char ch, std::vector<uint64_t>& bitboards);
};