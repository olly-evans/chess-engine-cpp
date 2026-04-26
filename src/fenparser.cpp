
#include "fenparser.hpp"


PieceInfo FenParser::parse_fen_char(const char ch) {

    
    PieceInfo info;
    info.color = isupper(ch) ? Color::WHITE : Color::BLACK;
    info.piece_id = toupper(ch);
    return info; 
}