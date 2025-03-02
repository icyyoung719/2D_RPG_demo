#pragma once 
#include <SFML/Graphics.hpp>
#include <optional>

struct Tile
{
    Tile() : texture(nullptr), sheetPosition(0, 0), isDynamic(false), currentLoopIndex(0) {}

    Tile(sf::Texture* texture, sf::Vector2i sheetPosition, bool isDynamic = false)
        : texture(texture), sheetPosition(sheetPosition), isDynamic(isDynamic) {
            sprite.emplace(*texture);
            sprite->setTextureRect(sf::IntRect({sheetPosition.x * 16, sheetPosition.y * 16}, {16, 16}));
        }

    void Update(float deltaTime){
        if(isDynamic){
            if(updateLoop.size() > 0){
                currentLoopIndex = (currentLoopIndex + 1) % updateLoop.size();
                sheetPosition = updateLoop[currentLoopIndex];
            }
        }

        if (sprite) {
            // resize to 4 times larger
            sprite->setScale(sf::Vector2f{4.0f,4.0f});
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
    sf::Vector2i sheetPosition;
    std::vector<sf::Vector2i> updateLoop;
    int currentLoopIndex;
};