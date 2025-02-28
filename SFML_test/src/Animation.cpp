#include "Animation.h"
#include <iostream>

Animation::Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, sf::Vector2i position, sf::Vector2i size) : 
    imageCount(imageCount), switchTime(switchTime), texture(texture), totalTime(0.0f), currentImage(0, 0)

{
    standardUvRect.position = sf::Vector2i(0, 0);
    standardUvRect.size.x = texture->getSize().x / float(imageCount.x);
    standardUvRect.size.y = texture->getSize().y / float(imageCount.y);

    textureRealUvRect.position = position;
    textureRealUvRect.size = size;
}

Animation::Animation():imageCount(0,0), switchTime(0.0f), totalTime(0.0f), currentImage(0,0)
{
}

Animation::~Animation()
{
}

void Animation::Update(int row, float deltaTime, bool faceRight)
{
    currentImage.y = row;
    totalTime += deltaTime;

    if (totalTime >= switchTime)
    {
        totalTime -= switchTime;
        currentImage.x++;
        if (currentImage.x >= imageCount.x)
        {
            currentImage.x = 0;
        }
    }

    standardUvRect.position.y = currentImage.y * standardUvRect.size.y;
    textureRealUvRect.position.y = standardUvRect.position.y + textureRealUvRect.position.y % abs(textureRealUvRect.size.y);


    if(faceRight){
        standardUvRect.position.x = currentImage.x * standardUvRect.size.x;
        standardUvRect.size.x = abs(standardUvRect.size.x);

        textureRealUvRect.position.x = standardUvRect.position.x + textureRealUvRect.position.x % abs(textureRealUvRect.size.x);
        textureRealUvRect.size.x = abs(textureRealUvRect.size.x);
    }
    else {

        standardUvRect.position.x = (currentImage.x + 1) * abs(standardUvRect.size.x);
        standardUvRect.size.x = -abs(standardUvRect.size.x);

        //textureRealUvRect.position.x = (currentImage.x + 1) * abs(textureRealUvRect.size.x);
        textureRealUvRect.position.x = standardUvRect.position.x - textureRealUvRect.position.x % abs(textureRealUvRect.size.x);
        textureRealUvRect.size.x = -abs(textureRealUvRect.size.x);
    }
}
