#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include "Tile.h"

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

    void Initialize(const std::string& jsonPath);
    void LoadMap(const std::string& jsonPath);
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
    std::unordered_map<sf::Vector2i, Tile> tileMap;

    // Helper function to add a tile to the map
    void addTile(sf::Vector2i mapPosition, Tile);
};


/*
    对于类似于背景的静态团，我们只保留一个全局的Sprite，因为他们不会改变，

    对于如箱子、矿石等，可能被单独改变状态的物体，我们需要对每个对象都创造一个Tile(包含Sprite),并注意编写Update函数

    算了，暂时我们还是将所有的都创建一个Tile吧，这样可以方便的对每个Tile进行操作，而且地图中也不会有太多的Tile

    后续可以优化
*/