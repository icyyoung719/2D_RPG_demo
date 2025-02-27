#pragma once
#include <SFML\Graphics.hpp>

class Animation
{
public:
    Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
    Animation();
    ~Animation();

    void Update(int row, float deltaTime, bool faceRight);
    sf::Texture* getTexture() { return texture; }

private:
    

public:
    sf::IntRect uvRect;

private:
    sf::Vector2u imageCount;
    sf::Vector2u currentImage;
    sf::Texture* texture;

    float totalTime;
    float switchTime;
};