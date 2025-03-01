#include "Animation.h"
#include <iostream>

Animation::Animation(const TextureInfo& textureInfo) : 
    textureInfo(textureInfo), totalTime(0.0f), currentImage(0, 0)
{
    standardUvRect.position = sf::Vector2i(0, 0);
    standardUvRect.size.x = static_cast<int>(textureInfo.texture->getSize().x / float(textureInfo.imageCount.x));
    standardUvRect.size.y = static_cast<int>(textureInfo.texture->getSize().y / float(textureInfo.imageCount.y));

    textureRealUvRect.position = textureInfo.position;
    textureRealUvRect.size = textureInfo.size;
}


Animation::Animation() : totalTime(0.0f), currentImage(0, 0)
{
}


Animation::~Animation()
{
}

void Animation::Update(int row, float deltaTime, bool faceRight)
{
    currentImage.y = row;
    totalTime += deltaTime;

    if (totalTime >= textureInfo.switchTime)
    {
        totalTime -= textureInfo.switchTime;
        currentImage.x++;
        if (currentImage.x >= textureInfo.imageCount.x)
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
