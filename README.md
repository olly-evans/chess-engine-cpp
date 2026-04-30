## Chess and Engine Programming

### Intro

So turns out to write a chess engine you first have to write the game of chess, I know right who would of thought. Writing chess the game so far has been much harder than I thought it would be, but has  also been very rewarding.

If you're asking why I didn't use a prebuilt chess GUI its because of my limited Object-Oriented programming experience. And Chess is for sure a great way to practise this with its board and piece hierarchies. But also because Dear Imgui and others in Visual Studio scares me to my core, so here I am.

For anyone cloning, I would recommend playing on the default 1280x1280 window width found in board.hpp. But it is up to you and the program will adapt dynamically.

### Goals

The goal for this project is to document my progress and learn more about C++, CMake and SFML. And also to become more familiar with common software design patterns.

I'll put the rest of my goals in a more formal list below, for readability:

- Discuss methods and techniques and why I used them
- Develop a functioning chess game with mouse movement and clicks
- Develop a competent AI to play against
- Develop a welcome screen to select black/white for the player and perhaps a welcome message
- Have fun and be creative

## Blog

If you'd like the full story I'd highly suggest checking out the blog for this project on my website where I have practically documented almost every step. And will continue to do so until completion.

The full blog is available here: [Blog](https://olly-evans.github.io/chess/)

## TODO List (I'm forgetful)

- Linux unsigned long is 64 bit.
- Should replace all int with uint_t.

- also now need some idea of a turn/team turn.

- perhaps draw circles for attacks instead of the fillcolors, will mean i dont have to fuck with squares.

- need global position to load fen.

- is_white_turn in moves bit.

- record/log moves potentially.

- for castling, occupancy call and & all bitboards.

- perhaps need some sort of gamestate class that holds whos turn, and game state like checkmate stalemate etc..

- extract fen token code to functions in fenparser. create_piece() can be part of something else perhaps.

- add global bitboard viewer to debug, overhaul piece cycle too to be more dynamic. Wanna init a window and use it to print debug shit.

- highlights shouldnt be full square, don't touch squares vector, make temp one whilst highlighting.
  then can just render the squares once but use same logic for highlight vector.

- change fenparser to load black from top and white from bottom regardless. // lord.

