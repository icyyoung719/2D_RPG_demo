#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>

namespace std {
    template <>
    struct hash<sf::Vector2i>
    {
        std::size_t operator()(const sf::Vector2i& v) const noexcept
        {
            return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
        }
    };
}

class Map
{
public:
    Map();
    ~Map();

    void Initialize();
    void LoadMap();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

private:
    int tileWidth;
    int tileHeight;
    int totalTilesX;
    int totalTilesY;

    sf::Texture tileSheetTexture;
    sf::Sprite tileSheetSprite;

    // store the map as a 2D array of tile positions
    std::unordered_map<sf::Vector2i, sf::Vector2i> tileMap;

    // Helper function to add a tile to the map
    void addTile(sf::Vector2i mapPosition, sf::Vector2i sheetPosition);
};


/*
    对于类似于背景的静态团，我们只保留一个全局的Sprite，因为他们不会改变，全局的Sprite需要注意编写Update，将
    会随着时间改变的动态效果，如树叶的飘动

    对于如箱子、矿石等，可能被单独改变状态的物体，我们需要对每个对象都创造一个Sprite
*/