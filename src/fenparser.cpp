
#include "fenparser.hpp"


PieceInfo FenParser::get_fen_char_info(const char ch) {
    PieceInfo info;
    info.color = isupper(ch) ? Color::WHITE : Color::BLACK;
    info.piece_id = toupper(ch);
    return info; 
}