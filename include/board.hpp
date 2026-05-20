#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System/String.hpp"
#include "SFML/Graphics/Image.hpp"

#include "pieces.hpp"
#include "debug.hpp"
#include "player.hpp"
#include "fenparser.hpp"

#include <sstream>
#include <iostream>
#include <cmath>
#include <array>

constexpr uint8_t NUM_PIECE_TYPES = 12; /* White rook, black pawn etc.. */


// class AbstractBoard {
//     virtual void die(std::string) = 0;
//     virtual void index_to_2d(int i) = 0;
//     .
//     .
//     .
// };

class Board {

private:

    /* GAMESTATE */
    
    // probably in app at some point.
    // Player* white_player;
    // Player* black_player;

    /* BITBOARDS */

    uint64_t w_pawns = 0ULL;
    uint64_t w_knights = 0ULL;
    uint64_t w_bishops = 0ULL;
    uint64_t w_rooks = 0ULL;
    uint64_t w_queen = 0ULL;
    uint64_t w_king = 0ULL;

    uint64_t b_pawns = 0ULL;
    uint64_t b_knights = 0ULL;
    uint64_t b_bishops = 0ULL;
    uint64_t b_rooks = 0ULL;
    uint64_t b_queen = 0ULL;
    uint64_t b_king = 0ULL;    

        /*

    uint64_t w_pawns bitboard initial positions example.

        a    b    c    d    e    f    g    h
    +----+----+----+----+----+----+----+----+
    | MSB|    |    |    |    |    |    |    | 8
    +----+----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    |    | 7
    +----+----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    |    | 6
    +----+----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    |    | 5
    +----+----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    |    | 4
    +----+----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    |    | 3
    +----+----+----+----+----+----+----+----+
    |  1 |  1 |  1 |  1 |  1 |  1 |  1 |  1 | 2
    +----+----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    | LSB| 1
    +----+----+----+----+----+----+----+----+

    */

public:


    bool is_whites_turn = true; /* Who's turn is it, allows user to select a piece depending. */
    
    Piece* selected_piece = nullptr; /* Pointer to user selected piece. */
    std::vector<std::shared_ptr<Piece>> pieces;

    static std::vector<uint64_t> bitboards; /* Mutable bitboards for each piece r,R,p,P etc.. */
    std::array<char, NUM_PIECE_TYPES> bitboard_names; /* Maps bitboard to a char r,R,p,P etc.. */

public:
    Board();

    /* INIT */

    void init();
    // void init_players();

    void load_position_from_fen(std::string fen);

    /* BITBOARD METHODS */

    uint64_t white_occupancy();
    uint64_t black_occupancy();

    uint64_t get_white_captures(uint64_t white, uint64_t black); 
    uint64_t get_black_captures(uint64_t white, uint64_t black); 
    uint64_t get_simulated_enemy_captures(Piece* piece, uint8_t start, uint8_t end, uint8_t capture);

    /* CHECK */

    bool white_king_in_check(uint64_t white, uint64_t black);
    bool black_king_in_check(uint64_t white, uint64_t black);

    void update_all_piece_attacks();

    void undo_move();

    /* PIECE FUNCTIONALITY */
    
    void create_piece(const char ch, uint8_t bit);
    Piece* select_piece(uint8_t bit);
    Piece* get_piece(uint8_t clicked_bit);
    void remove_piece(uint8_t piece_to_remove_bit);

    bool bit_has_piece(uint8_t clicked_bit);

    void handle_piece_move(uint8_t clicked_bit);
    bool is_enpassant_capture(uint8_t clicked_bit);
};