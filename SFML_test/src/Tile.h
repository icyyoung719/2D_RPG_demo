#pragma once 
#include <SFML/Graphics.hpp>
#include <optional>

struct Tile
{
    Tile() : texture(nullptr), sheetPosition(0, 0), isDynamic(false), currentLoopIndex(0) {}

    Tile(sf::Texture* texture, sf::Vector2i sheetPosition, bool isDynamic = false, float switchTime = 0.0f)
        : texture(texture), sheetPosition(sheetPosition), isDynamic(isDynamic), switchTime(switchTime), totalTime(0.0f) {
            sprite.emplace(*texture);
            sprite->setTextureRect(sf::IntRect({sheetPosition.x * 16, sheetPosition.y * 16}, {16, 16}));
        }

    void Update(float deltaTime){
        if(isDynamic){
            totalTime += deltaTime;
            if(totalTime >= switchTime){
                totalTime -= switchTime;
                if(updateLoop.size() > 0){
                    currentLoopIndex = (currentLoopIndex + 1) % updateLoop.size();
                    sheetPosition = updateLoop[currentLoopIndex];
                }
            }
        }

        if (sprite) {
            sprite->setScale(sf::Vector2f{4.0f, 4.0f});
            sprite->setTextureRect(sf::IntRect({sheetPosition.x * 16, sheetPosition.y * 16}, {16, 16}));
        }
    }
    

    void setUpdateLoop(std::vector<sf::Vector2i> loop){
        updateLoop = loop;
        auto it = std::find(updateLoop.begin(), updateLoop.end(), sheetPosition);
        if(it == updateLoop.end()){
            sheetPosition = updateLoop[0];
        } else {
            currentLoopIndex = static_cast<int>(std::distance(updateLoop.begin(), it));
        }
    }

    bool isDynamic;
    sf::Texture* texture;
    std::optional<sf::Sprite> sprite;
    // change with time, show current position in the tilesheet
    sf::Vector2i sheetPosition;
    std::vector<sf::Vector2i> updateLoop;
    int currentLoopIndex;
    float switchTime;
    float totalTime;
};