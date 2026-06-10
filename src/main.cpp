#include <iostream>

#include "board.hpp"
#include "sfml_app.hpp"

// const std::string FEN = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"; // Complex check check.
// const std::string FEN = "8/8/8/4k3/8/4P3/4K3/8 w - - 0 1"; // King-pawn endgame.
// const std::string FEN = "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1"; // Discovered en passant check.
const std::string FEN = "r3k2r/ppp1p1pp/8/4Pp2/1PpP3/8/PPPP1PPP/R3K2R b KQkq d3 0 3";

int main() {

    Board board(FEN);
    SFMLApp app(board, WINDOW_WIDTH);

    app.init();
    app.run();

    return 0;
}