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
    sf::Vector2i startPixel;
    // the real model size
    sf::Vector2i modelSize;
    // the png size
    sf::Vector2i pngSize;

    TextureInfo(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, sf::Vector2i startPixel, sf::Vector2i modelSize)
        : texture(texture), imageCount(imageCount), switchTime(switchTime), startPixel(startPixel), modelSize(modelSize)
    {
        pngSize = sf::Vector2i(texture->getSize().x / imageCount.x, texture->getSize().y / imageCount.y);
    }

    TextureInfo() : texture(nullptr), imageCount(sf::Vector2u(0, 0)), switchTime(0.0f), startPixel(sf::Vector2i(0, 0)), modelSize(sf::Vector2i(0, 0)), pngSize(sf::Vector2i(0, 0)) {}
};