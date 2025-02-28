#pragma once
#include <SFML/Graphics.hpp>

struct TextureInfo
{
    sf::Texture* texture;
    // the number of images(frame) in the texture
    sf::Vector2u imageCount;
    // the time between each frame
    float switchTime;
    // where the real model starts
    sf::Vector2i position;
    // the real model size
    sf::Vector2i size;
};