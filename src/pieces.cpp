#include "util.hpp"
#include "pieces.hpp"
#include "board.hpp"
#include "bitboardhelper.hpp"

#include <iostream>
#include <filesystem>
#include <cstdint>

std::string Piece::resolve_texture_path() {
    std::filesystem::path path = std::filesystem::current_path();
    std::string color_prefix = (color == Color::WHITE ? "w" : "b");
    return path.string() + "/assets/" + color_prefix + piece_id + ".png";
}

Piece::Piece(std::string id, Color col, sf::RenderWindow& w, uint16_t s_squ_idx, int b_squ_sz) : 
    piece_id(std::move(id)), 
    color(col),
    window(w), 
    start_square_index(s_squ_idx),
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

void Piece::highlight_legal_moves(std::vector<sf::Vector2f> legal_moves, std::vector<sf::RectangleShape>& squares) {
    
    std::vector<uint16_t> indexes{};

    for (int i = 0; i < legal_moves.size(); i++) {indexes.emplace_back(pos2d_to_index(legal_moves[i]));}

    for (int i = 0; i < indexes.size(); i++) {squares[indexes[i]].setFillColor(TURQOISE);}

}

/* PAWN */

uint64_t Pawn::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* KNIGHT */

bool Knight::is_knight_move_on_board(sf::Vector2f piece_pos, int move_dx, int move_dy) {
    int new_x = piece_pos.x + move_dx;
    int new_y = piece_pos.y + move_dy;
    return (new_x >= 0 && new_x < GRID_SZ && new_y >= 0 && new_y < GRID_SZ);
}

uint64_t Knight::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {
    
    int square = pos2d_to_index(this->pos);
    int bit = BitboardHelper::square_to_bit(square);

    uint64_t knight = 0ULL << bit;
    uint64_t attacks;
    
    attacks |= (knight << 17);
    attacks |= (knight << 15);
    attacks |= (knight << 10);
    attacks |= (knight << 6);

    attacks |= (knight >> 17);
    attacks |= (knight >> 15);
    attacks |= (knight >> 10);
    attacks |= (knight >> 6);


    return BitboardHelper::remove_friendly_pieces(attacks, this->color == Color::WHITE ? w_bb : b_bb);
        // Checks.
        // if (!is_knight_move_on_board(this->pos, Knight::offsets[i][0], Knight::offsets[i][1])) continue;
    // if (this->color == Color::WHITE && BitboardHelper::has_friendly_piece(w_bb, move_square)) continue;
    // if (this->color == Color::BLACK && BitboardHelper::has_friendly_piece(b_bb, move_square)) continue;

}



/* BISHOP */

uint64_t Bishop::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* ROOK */

uint64_t Rook::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* QUEEN */
uint64_t Queen::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};

/* KING */

uint64_t King::set_legal_moves(uint64_t w_bb, uint64_t b_bb) {};