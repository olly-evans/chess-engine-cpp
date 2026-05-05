
#include "fenparser.hpp"


FenCharInfo FenParser::get_fen_char_info(const char ch) {
    FenCharInfo info;
    info.color = isupper(ch) ? Color::WHITE : Color::BLACK;
    info.piece_id = ch;
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

std::vector<std::string> FenParser::split_with_delimiter(std::string s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}

    
uint64_t& FenParser::get_fen_char_bitboard(char ch, std::vector<uint64_t>& bitboards) {

    /* Split was giving garbage values so had to guard the return here */

    static uint64_t dummy = 0ULL;
    int index = -1;

    switch (ch) {
        case 'P': index = W_PAWNS; break;
        case 'N': index = W_KNIGHTS; break;
        case 'B': index = W_BISHOPS; break;
        case 'R': index = W_ROOKS; break;
        case 'Q': index = W_QUEEN; break;
        case 'K': index = W_KING; break;
    
        case 'p':index = B_PAWNS; break;
        case 'n': index = B_KNIGHTS; break;
        case 'b': index = B_BISHOPS; break;
        case 'r': index = B_ROOKS; break;
        case 'q': index = B_QUEEN; break;
        case 'k': index = B_KING; break;
        default: return dummy; 
    }

    if (index < 0 || index >= (int)bitboards.size()) return dummy;

    return bitboards[index];
    
}


