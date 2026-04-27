
#include "fenparser.hpp"


PieceInfo FenParser::get_fen_char_info(const char ch) {
    PieceInfo info;
    info.color = isupper(ch) ? Color::WHITE : Color::BLACK;
    info.piece_id = toupper(ch);
    return info; 
}

std::vector<std::string> FenParser::split(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

