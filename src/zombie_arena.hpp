#pragma once

#include <SFML/Graphics.hpp>
#include "zombie.hpp"

#include <vector>

int create_background(sf::VertexArray& va, sf::IntRect arena);
std::vector<Zombie> create_horde(int32_t num_zombies, sf::IntRect arena);