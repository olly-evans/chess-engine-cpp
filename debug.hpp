#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <cstdint>

class Debug {
public:
    // Toggle this to false to stop debug toggles.
    inline static bool enabled = true;
    
    template<typename T>
    static void run(T task) {
        if (enabled)
            task(); 
    }

    /* DEBUG METHODS */
    
    static void draw_cycle_bitboard(sf::RenderWindow &window, 
                                    int win_w, 
                                    int win_h,
                                    std::vector<uint64_t> bitboards, 
                                    std::vector<std::string> board_names, 
                                    int bitboard_vec_index, 
                                    std::vector<sf::RectangleShape> &squares);

    static void show_mouse_pos(int x, int y);
    
    static void log(const std::string& message);
};

#endif