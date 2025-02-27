#include "Animation.h"


Animation::Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime)
{
    this->imageCount = imageCount;
    this->switchTime = switchTime;
    this->texture = texture;
    totalTime = 0.0f;
    currentImage.x = 0;

    uvRect.size.x = texture->getSize().x / float(imageCount.x);
    uvRect.size.y = texture->getSize().y / float(imageCount.y);
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

    uvRect.position.y = currentImage.y * uvRect.size.y;

    if(faceRight){
        uvRect.position.x = currentImage.x * uvRect.size.x;
        uvRect.size.x = abs(uvRect.size.x);
    }
    else {
        uvRect.position.x = (currentImage.x + 1) * abs(uvRect.size.x);
        uvRect.size.x = -abs(uvRect.size.x);
    }


}
