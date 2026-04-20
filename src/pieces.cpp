
#include "util.hpp"
#include "pieces.hpp"

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
        sprite.setPosition(pos.x, pos.y);
        window.draw(sprite);
    }
}

void Piece::init_piece_positions_vector_from_bitboard(uint64_t bitboard, std::vector<sf::Vector2f> &piece_pos) {

    // init_positions_vector_from_bitboard();
    for (int i = 0; i < GRID_NUM_SQUARES; i++) {
        if (bitboard & (1ULL << i)) {
            int draw_idx = GRID_NUM_SQUARES - i - 1;
            piece_pos.push_back(index_to_2d(draw_idx) * (float)board_square_size);
        }
    }
}

void Piece::render_highlight(sf::Vector2f piece_position) {
    
    std::cout << "highlight: " << piece_position.x << ", " << piece_position.y <<  "\n";

    for (auto pos : piece_positions) {
        pos.x = pos.x / board_square_size;
        pos.y = pos.y / board_square_size;

        std::cout << "piece_pos: " << pos.x << ", " << pos.y << "\n";
        // if (pos.x == piece_position.x && pos.y == piece_position.y);


        // render the highlight.
    }
}
