#pragma once

#include <SFML/Graphics.hpp>
#include <map>

class TextureHolder
{
public:
    static TextureHolder& getInstance()
    {
        static TextureHolder instance_;
        return instance_;
    }
    ~TextureHolder();
    TextureHolder(const TextureHolder& other) = delete;
    void operator=(const TextureHolder& other) = delete;

private:
    TextureHolder();

public:
    sf::Texture& GetTexture(const std::string& filename);

private:
    std::map<std::string, sf::Texture> textures_;
};