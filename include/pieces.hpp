#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdint>

class Board; // this is only here for strip pseudo legal moves.

class Piece {

protected:

    sf::Texture texture;

public:
    bool is_white; /* Color of piece bool assigned in constructor for readability as opposed to isupper(piece->id) etc..*/

    sf::Sprite sprite; /* Assigned sprite to draw for this piece from SFMLApp::texture_cache. */

    char id; /* Contains piece and its color, r = Black Rook and R = White Rook as in fen notation. */

    uint64_t moves = 0ULL; /* Bits a piece can move to with no captuere. */
    uint64_t captures = 0ULL; /* A bitboard of active bits a piece can capture an enemy piece on. */
    uint8_t bit; /* Where the piece is on a bitboard, where bit 0 is H1 and 8 is H2. */

public:
    Piece(char id, uint8_t b);

    void set_bit(uint8_t bit);
    uint8_t get_bit();

    // All instances return the moves and assign this->captures.
    virtual void set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) = 0;

    void strip_pseudo_legal_attacks(Board& board);
    virtual void strip_pseudo_legal_special_moves(Board& board) = 0;
    
    uint64_t get_north_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_south_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_west_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_east_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);

    uint64_t get_north_west_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_north_east_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_south_west_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_south_east_moves(uint64_t piece, uint64_t w_bb, uint64_t b_bb);

};

class Pawn : public Piece {
public:
    Pawn(char id, uint8_t bitboard) : 
        Piece(id, bitboard) {}
    
    uint64_t en_passant_capture_bit = 0ULL;

    void set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) override;

    void strip_pseudo_legal_special_moves(Board& board) override;

    uint64_t get_white_pawn_moves(uint64_t pawn, uint64_t w_bb, uint64_t b_bb);
    uint64_t get_black_pawn_moves(uint64_t pawn, uint64_t w_bb, uint64_t b_bb);

    uint64_t get_enpassant(uint64_t w_bb, uint64_t b_bb);
};

class Knight : public Piece {
public:

    Knight(char id, uint8_t b) : 
        Piece(id, b) {}
    
    void set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) override;
        
    void strip_pseudo_legal_special_moves(Board& board) override;

};

class Bishop : public Piece {
public:
    Bishop(char id, uint8_t b) : 
        Piece(id, b) {}
    
    void set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) override;

    void strip_pseudo_legal_special_moves(Board& board) override;
};

class Rook : public Piece {
public:
    Rook(char id, uint8_t b) : 
        Piece(id, b) {}

    void set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) override;
    
    void strip_pseudo_legal_special_moves(Board& board) override;
};

class Queen : public Piece {
public:
    Queen(char id, uint8_t b) : 
        Piece(id, b) {}

    void set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) override;

    void strip_pseudo_legal_special_moves(Board& board) override;
};

class King : public Piece {
public:
    King(char id, uint8_t b) : 
        Piece(id, b) {}

    void set_pseudo_legal_attacks(uint64_t w_bb, uint64_t b_bb) override;
    
    void strip_pseudo_legal_special_moves(Board& board) override;

    bool can_pseudo_legal_queenside_castle(uint64_t w_bb, uint64_t b_bb);
};