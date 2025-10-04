#include "texture_holder.hpp"
#include <iostream>

TextureHolder::TextureHolder()
{

}

TextureHolder::~TextureHolder()
{

}

sf::Texture& TextureHolder::GetTexture(const std::string& filename)
{
    auto entity = textures_.find(filename);
    if (entity != textures_.end()) {
        return entity->second;
    } else {
        auto& texture = textures_[filename];
        if (!texture.loadFromFile(filename)) {
            std::cout << "failed to load texture from file: " << filename << std::endl;
        }
        return texture;
    }
}