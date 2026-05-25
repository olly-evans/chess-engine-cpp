#include "fenparser.hpp"
#include "board.hpp"
#include "bitboardhelper.hpp"

#include "pieces/pawn.hpp"

FenParser::FenParser() {}

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

    
uint64_t& FenParser::get_fen_char_bitboard(char ch, std::array<uint64_t, NUM_PIECE_TYPES>& bitboards) {

    static uint64_t dummy = 0ULL;
    int index = -1;

    switch (ch) {
        case 'P': index = W_PAWNS;   break;
        case 'N': index = W_KNIGHTS; break;
        case 'B': index = W_BISHOPS; break;
        case 'R': index = W_ROOKS;   break;
        case 'Q': index = W_QUEEN;   break;
        case 'K': index = W_KING;    break;
    
        case 'p': index = B_PAWNS;   break;
        case 'n': index = B_KNIGHTS; break;
        case 'b': index = B_BISHOPS; break;
        case 'r': index = B_ROOKS;   break;
        case 'q': index = B_QUEEN;   break;
        case 'k': index = B_KING;    break;
        default: return dummy; 
    }

    if (index < 0 || index >= NUM_PIECE_TYPES) 
        return dummy;

    return bitboards.at(index);
}

/* PARSE */

void FenParser::parse_fen_position(Board& board, std::string fen_pos_sub_str) {
    uint8_t rank = 7, file = 0;
    for (char ch : fen_pos_sub_str) {

        if (ch == '/') {
            rank--;
            file = 0;
        } else if (isdigit(ch)) {
            file += ch - '0';
        } else if (isalpha(ch)) {
            uint8_t bit = rank * 8 + (7 - file);

            board.create_piece(ch, bit);
            file++;

            // Get correct piece type bitboard from ch.
            uint64_t& bitboard = FenParser::get_fen_char_bitboard(ch, board.bitboards);
            BBHelper::set_bit_by_ref(bitboard, bit);
        }
    }
}

void FenParser::parse_and_set_fen_enpassant(Board& board, std::string ep_target) {
    
    /* 
    *  Take "e4" for example, as a string and highlight it as an enpassant capture for one turn
    *  in enpassant_from_fen pawn uint64_t member.
    *
    */

    if (ep_target.length() > 2)
        std::cerr << "Invalid fen En Passant square" << "\n";

    if (ep_target == "-")
        return;
    
    uint8_t fen_bit = BBHelper::square_name_to_bit(ep_target);    
    
    uint8_t left_offset = (board.is_whites_turn) ? - 7 : 9;
    uint8_t left = fen_bit + left_offset;

    uint8_t right_offset = (board.is_whites_turn) ? - 9 : 7;
    uint8_t right = fen_bit + right_offset;

    uint64_t pawns = (board.is_whites_turn) ? board.bitboards[W_PAWNS] 
                                            : board.bitboards[B_PAWNS];

    bool capturing_pawn_left = pawns & (1ULL << left);
    bool capturing_pawn_right = pawns & (1ULL << right);

    if (!capturing_pawn_left && !capturing_pawn_right) 
        return;
        
    uint8_t capture_bit = (board.is_whites_turn) ? fen_bit - 8 : fen_bit + 8; 
    if (capturing_pawn_left) {
        std::shared_ptr<Pawn> pawn_south_west = std::dynamic_pointer_cast<Pawn>(board.get_piece(left));
        
        // std::cout << "fen, left pawn bit" << pawn_south_west->bit << "\n";
        BBHelper::set_bit_by_ref(pawn_south_west->captures, fen_bit);
        BBHelper::set_bit_by_ref(pawn_south_west->en_passant_capture_bit, capture_bit);
        BBHelper::set_bit_by_ref(pawn_south_west->enpassant_from_fen, fen_bit);
    } else if (capturing_pawn_right) {
        std::shared_ptr<Pawn> pawn_south_east = std::dynamic_pointer_cast<Pawn>(board.get_piece(right));
        // std::cout << "fen, right pawn bit" << pawn_south_east->bit << "\n";

        BBHelper::set_bit_by_ref(pawn_south_east->captures, fen_bit);
        BBHelper::set_bit_by_ref(pawn_south_east->en_passant_capture_bit, capture_bit);
        BBHelper::set_bit_by_ref(pawn_south_east->enpassant_from_fen, fen_bit);
    }
}

void FenParser::parse_and_set_fen_castling_rights(Board& board, std::string castling_rights) {

    // store the bitboard of the loaded pos perhaps.
    // just in case.
    if (castling_rights == "-")
        return; // castling_rights = 0;

    if (castling_rights.length() > 4)
        std::cerr << "Castling rights in fen must contain 4 characters or less.";

    for (char ch : castling_rights) {
        switch (ch) {
            case 'K':
                board.set_castling_rights_bit(3);
                break;
            case 'k':
                board.set_castling_rights_bit(2);
                break;
            case 'Q':
                board.set_castling_rights_bit(1);
                break;
            case 'q':
                board.set_castling_rights_bit(0);
                break;
        }
    }
}