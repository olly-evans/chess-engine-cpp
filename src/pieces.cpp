
#include "util.hpp"
#include "pieces.hpp"
#include "board.hpp"

#include <iostream>
#include <filesystem>
#include <cstdint>



std::string Piece::resolve_texture_path() {

    // Change assets to 160px when bothered. In folder.
    std::filesystem::path path = std::filesystem::current_path();
    std::string color_prefix = (color == Color::WHITE ? "w" : "b");
    return path.string() + "/assets/" + color_prefix + piece_id + ".png";
}

Piece::Piece(std::string id, Color col, sf::RenderWindow& w, uint64_t bitb, int b_squ_sz) : 
    piece_id(std::move(id)), 
    color(col),
    window(w), 
    bitboard(bitb),
    board_square_size(b_squ_sz) {
        
    init_piece_positions_vector_from_bitboard(bitboard, piece_positions);
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

    for (const auto& pos : piece_positions) {
        sprite.setPosition(pos.vec.x, pos.vec.y);
        window.draw(sprite);
    }
}

void Piece::init_piece_positions_vector_from_bitboard(uint64_t bitboard, std::vector<Pos> &piece_pos) {

    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        if (bitboard & (1ULL << i)) {
            int draw_idx = GRID_NUM_SQUARES - i - 1;
            sf::Vector2f pos = index_to_2d(draw_idx) * (float)board_square_size;
            Pos piece_data = {pos, false};
            // mouses pos coords not squares on grid coords.

            piece_pos.push_back(piece_data);
        }
    }
}

void Piece::make_highlighted() {
    is_highlighted = !is_highlighted;
}

void Piece::render_highlight(sf::Vector2f piece_pos, std::vector<sf::RectangleShape>& squares, bool& piece_highlight_active) {
    
    for (int i = 0; i < piece_positions.size(); i++) {

        sf::Vector2f stored_pos = piece_positions[i].vec;

        stored_pos.x = stored_pos.x / board_square_size;
        stored_pos.y = stored_pos.y / board_square_size;
        
        if (!piece_highlight_active && is_vecs_equal(stored_pos, piece_pos)) {
            piece_highlight_active = true;
            int index = pos2d_to_index(piece_pos);
            squares[index].setFillColor(TURQOISE);
            this->piece_positions[i].is_highlighted = true;
        }
            
    }
}

