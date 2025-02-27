#pragma once
#include <SFML\Graphics.hpp>

class Animation
{
public:
    Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, sf::Vector2i position, sf::Vector2i size);
    Animation();
    ~Animation();

    void Update(int row, float deltaTime, bool faceRight);
    sf::Texture* getTexture() { return texture; }

private:
    

public:
    // determine the position and size of the texture(despites the real size of the texture)
    sf::IntRect standardUvRect;
    // real model size after removing the transparent part
    sf::IntRect textureRealUvRect;

private:
    sf::Vector2u imageCount;
    sf::Vector2u currentImage;
    sf::Texture* texture;

    float totalTime;
    float switchTime;
};