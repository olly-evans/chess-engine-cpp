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

- toggle mouse pos in console too.

- Linux unsigned long is 64 bit.
- Should replace all int with uint_t.

- also now need some idea of a turn/team turn.

- perhaps draw circles for attacks instead of the fillcolors, will mean i dont have to fuck with squares.

- when you remake the focus the main_window, if you click on a piece w/o get_legal_moves crashes.
