#include "gamestate.hpp"


void GameState::set_fen_pos(std::string fen) {
    // is valid fen
    GameState::fen_pos = fen;
}

std::string GameState::get_fen_pos() {
    return GameState::fen_pos;
}

