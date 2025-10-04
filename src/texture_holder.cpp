#include "texture_holder.hpp"

TextureHolder::TextureHolder()
{

}

TextureHolder::~TextureHolder()
{

}

sf::Texture TextureHolder::GetTexture(const std::string& filename)
{
    auto entity = textures_.find(filename);
    if (entity != textures_.end()) {
        return entity->second;
    } else {
        auto& texture = textures_[filename];
        texture.loadFromFile(filename);
        return texture;
    }
}