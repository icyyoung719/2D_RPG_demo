#include "Map.h"
#include <iostream>
#include <string>

Map::Map() : 
    tileSheetSprite(tileSheetTexture), tileWidth(16), tileHeight(16), totalTilesX(0), totalTilesY(0), tileMap{}
{
}

Map::~Map()
{
}

void Map::Initialize()
{
    addTile({0, 0}, {0, 0});
    addTile({1, 0}, {1, 0});
    addTile({2, 0}, {2, 0});
    addTile({0, 1}, {0, 1});
    addTile({1, 1}, {1, 1});
    addTile({2, 1}, {2, 1});
    addTile({0, 2}, {0, 0});
    addTile({1, 2}, {0, 0});
    addTile({2, 2}, {0, 0});
}

void Map::LoadMap()
{
    std::string path = "../../../../SFML_test/assests/World/Overworld.png";
    if(tileSheetTexture.loadFromFile(path))
    {
        totalTilesX = tileSheetTexture.getSize().x / tileWidth;
        totalTilesY = tileSheetTexture.getSize().y / tileHeight;
        tileSheetSprite.setTexture(tileSheetTexture);
        tileSheetSprite.setTextureRect(sf::IntRect({0, 0}, {tileWidth, tileHeight}));
        tileSheetSprite.setPosition({0, 0});
        tileSheetSprite.setScale({4.0f, 4.0f}); 

        std::cout << "World Map Texture Loaded" << std::endl;
        std::cout << "Total Tiles X: " << totalTilesX << std::endl;
        std::cout << "Total Tiles Y: " << totalTilesY << std::endl;
    }
    else
    {
        std::cerr << "Error loading texture" << std::endl;
    }
}

void Map::Update(float deltaTime)
{
}

void Map::Draw(sf::RenderWindow &window)
{
    for (const auto& tile : tileMap)
    {
        sf::Vector2i mapPosition = tile.first;
        sf::Vector2i sheetPosition = tile.second;
        // make sure the tile is within the bounds of the tile sheet
        if (sheetPosition.x >= totalTilesX || sheetPosition.y >= totalTilesY)
        {
            std::cerr << "Tile out of bounds" << std::endl;
            continue;
        }

        tileSheetSprite.setPosition({mapPosition.x * tileWidth * 4.0f, mapPosition.y * tileHeight * 4.0f});
        tileSheetSprite.setTextureRect(sf::IntRect({sheetPosition.x * tileWidth, sheetPosition.y * tileHeight}, {tileWidth, tileHeight}));
        window.draw(tileSheetSprite);
    }
}

void Map::addTile(sf::Vector2i mapPosition, sf::Vector2i sheetPosition)
{
    tileMap[mapPosition] = sheetPosition;
}