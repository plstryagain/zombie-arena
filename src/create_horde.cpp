#include "zombie_arena.hpp"
#include <iostream>

enum class SIDE : int32_t
{
    kLeft = 0,
    kRight = 1,
    kTop = 2,
    kBottom = 3
};

std::vector<Zombie> create_horde(int32_t num_zombies, sf::IntRect arena)
{
    std::vector<Zombie> zombies;
    zombies.resize(num_zombies);
    int32_t max_y = arena.height - 20;
    int32_t min_y = arena.top + 20;
    int32_t max_x = arena.width + 20;
    int32_t min_x = arena.left - 20;
    for (int32_t i = 0; i < num_zombies; ++i) {
        srand(static_cast<int32_t>(time(0)) * i);
        SIDE side = static_cast<SIDE>(rand() % 4);
        float x, y = 0;
        switch (side) {
        case SIDE::kLeft:
            x = min_x;
            y = (rand() % max_y) + min_y;
            break;
        case SIDE::kRight:
            x = max_x;
            y = (rand() % max_y) + min_y;
            break;
        case SIDE::kTop:
            x = (rand() % max_x) + min_x;
            y = min_y;
            break;
        case SIDE::kBottom:
            x = (rand() % max_x) + min_x;
            y = max_y;
            break;
        }
        std::cout << "side: " << static_cast<int32_t>(side) << std::endl;
        srand(static_cast<int32_t>(time(0)) * i * 2);
        Zombie::TYPE type = static_cast<Zombie::TYPE>(rand() % 3);
        zombies[i].spawn(x, y, type, i);
    }
    return zombies;
}