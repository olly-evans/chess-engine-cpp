
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
};