#include "util.hpp"
#include "pieces.hpp"
#include "board.hpp"

#include <iostream>
#include <filesystem>
#include <cstdint>

std::string Piece::resolve_texture_path() {
    std::filesystem::path path = std::filesystem::current_path();
    std::string color_prefix = (color == Color::WHITE ? "w" : "b");
    return path.string() + "/assets/" + color_prefix + piece_id + ".png";
}

Piece::Piece(std::string id, Color col, sf::RenderWindow& w, uint16_t squ_idx, int b_squ_sz) : 
    piece_id(std::move(id)), 
    color(col),
    window(w), 
    start_square_index(squ_idx),
    board_square_size(b_squ_sz) {
    

    // take squ_idx and convert to 2d normalised pos.
    pos = index_to_2d(start_square_index);

    // we then never touch index again.
    if (texture.loadFromFile(get_texture_path())) {
        sprite.setTexture(texture);

        sf::FloatRect sprite_size = sprite.getGlobalBounds();
        sprite.setScale(board_square_size / sprite_size.width, 
                        board_square_size / sprite_size.height);
    }
}

std::string Piece::get_texture_path() {
    return resolve_texture_path();
}

void Piece::draw(sf::RenderWindow& window) {
    /*
        For a piece type (i.e White Rook or Black Bishop), draw all active pieces
        on the board in their appropriate position.
    */

    sf::Vector2f window_pos = this->pos * (float)board_square_size;
    sprite.setPosition(window_pos.x, window_pos.y);
    window.draw(sprite);
}

void Piece::render_highlight(sf::Vector2f clicked_pos, std::vector<sf::RectangleShape>& squares) {
    
    if (is_vecs_equal(clicked_pos, this->pos)) {
        int index = pos2d_to_index(this->pos);
        squares[index].setFillColor(TURQOISE);
    }
}

bool Piece::move_square_has_friendly_piece(uint64_t w_bitboard, 
                                           uint64_t b_bitboard, 
                                           Color col, 
                                           sf::Vector2f move) {
    
    int index = pos2d_to_index(move);
    int square_idx = GRID_NUM_SQUARES - index - 1;

    if (w_bitboard & (1ULL << square_idx) && col == Color::WHITE) return true;
    if (b_bitboard & (1ULL << square_idx) && col == Color::BLACK) return true;
    return false;
}

void Piece::highlight_legal_moves(std::vector<sf::Vector2f> legal_moves, std::vector<sf::RectangleShape>& squares) {
    
    std::vector<uint16_t> indexes{};

    for (int i = 0; i < legal_moves.size(); i++) {indexes.emplace_back(pos2d_to_index(legal_moves[i]));}

    for (int i = 0; i < indexes.size(); i++) {squares[indexes[i]].setFillColor(TURQOISE);}

}

/* PAWN */

void Pawn::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* KNIGHT */

bool Knight::is_knight_move_on_board(sf::Vector2f piece_pos, int move_dx, int move_dy) {
    int new_x = piece_pos.x + move_dx;
    int new_y = piece_pos.y + move_dy;
    return (new_x >= 0 && new_x < GRID_SZ && new_y >= 0 && new_y < GRID_SZ); 
}

void Knight::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {

    // const and can put somewhere else.
    int num_offsets = sizeof(Knight::offsets) / sizeof(Knight::offsets[0]);
    
    std::vector<sf::Vector2f> legal_moves;

    for (int i = 0; i < num_offsets; i++) {

        int new_x = this->pos.x + Knight::offsets[i][0];
        int new_y = this->pos.y + Knight::offsets[i][1];

        sf::Vector2f move(new_x, new_y);

        if (!is_knight_move_on_board(this->pos, Knight::offsets[i][0], Knight::offsets[i][1])) continue;
        if (move_square_has_friendly_piece(w_bb, b_bb, this->color, move)) continue;

        legal_moves.push_back(move);
    }   
    this->legal_moves = legal_moves;     
}

/* BISHOP */

void Bishop::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* ROOK */

void Rook::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* QUEEN */
void Queen::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* KING */

void King::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};