#include "zombie_arena.hpp"

int create_background(sf::VertexArray& va, sf::IntRect arena)
{
    const int32_t TILE_SIZE = 50;
    const int32_t TILE_TYPES = 3;
    const int32_t VERTS_IN_QUAD = 4;
    int32_t world_width = arena.width / TILE_SIZE;
    int32_t world_height = arena.height / TILE_SIZE;
    va.setPrimitiveType(sf::Quads);
    va.resize(world_width * world_height * VERTS_IN_QUAD);
    int32_t current_vertex = 0;
    for (int32_t w = 0; w < world_width; ++w) {
        for (int32_t h = 0; h < world_height; ++h) {
            va[current_vertex].position = sf::Vector2f{static_cast<float>(w * TILE_SIZE), static_cast<float>(h * TILE_SIZE)};
            va[current_vertex + 1] = sf::Vector2f{static_cast<float>((w * TILE_SIZE) + TILE_SIZE), static_cast<float>(h * TILE_SIZE)};
            va[current_vertex + 2] = sf::Vector2f{static_cast<float>((w * TILE_SIZE) + TILE_SIZE), static_cast<float>((h * TILE_SIZE) + TILE_SIZE)};
            va[current_vertex + 3] = sf::Vector2f{static_cast<float>(w * TILE_SIZE),static_cast<float>((h * TILE_SIZE) + TILE_SIZE)};

            if (h == 0 || h == world_height - 1 || w == 0 || w == world_width -1) {
                // wall texture
                va[current_vertex].texCoords = sf::Vector2f{0, 0 + TILE_TYPES * TILE_SIZE};
                va[current_vertex + 1].texCoords = sf::Vector2f{TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE};
                va[current_vertex + 2].texCoords = sf::Vector2f{TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE};
                va[current_vertex + 3].texCoords = sf::Vector2f{0, TILE_SIZE + TILE_TYPES * TILE_SIZE};
            } else {
                srand(static_cast<int>(time(0)) + h * w - h);
                int32_t mud_or_grass = rand() % TILE_TYPES;
                int32_t vertical_offset = mud_or_grass * TILE_SIZE;
                va[current_vertex].texCoords = sf::Vector2f{0, static_cast<float>(0 + vertical_offset)};
                va[current_vertex + 1].texCoords = sf::Vector2f{TILE_SIZE, static_cast<float>(0 + vertical_offset)};
                va[current_vertex + 2].texCoords = sf::Vector2f{TILE_SIZE, static_cast<float>(TILE_SIZE + vertical_offset)};
                va[current_vertex + 3].texCoords = sf::Vector2f{0, static_cast<float>(TILE_SIZE + vertical_offset)};
            }

            current_vertex += VERTS_IN_QUAD;
        }
    }
    return TILE_SIZE;
}