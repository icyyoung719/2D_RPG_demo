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
    LoadMap();
    Tile test1{&tileSheetTexture, {0, 0}};
    Tile test2{&tileSheetTexture, {1, 0}};
    Tile test3{&tileSheetTexture, {2, 0}};
    Tile test4{&tileSheetTexture, {0, 1}};

    // addTile({0, 0}, {0, 0});
    // addTile({1, 0}, {1, 0});
    // addTile({2, 0}, {2, 0});
    // addTile({0, 1}, {0, 1});
    // addTile({1, 1}, {1, 1});
    // addTile({2, 1}, {2, 1});
    // addTile({0, 2}, {0, 0});
    // addTile({1, 2}, {0, 0});
    // addTile({2, 2}, {0, 0});

    addTile({0, 0}, test1);
    addTile({1, 0}, test2);
    addTile({2, 0}, test3);
    addTile({0, 1}, test4);
    addTile({1, 1}, test1);
    addTile({2, 1}, test2);
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
    for (auto& tile : tileMap)
    {
        tile.second.Update(deltaTime);
    }
}

void Map::Draw(sf::RenderWindow &window)
{
    for (const auto& tile : tileMap)
    {
        if (tile.second.sprite) {
            window.draw(*tile.second.sprite);
        }
    }
}

void Map::addTile(sf::Vector2i mapPosition, Tile targetTile)
{
    targetTile.sprite->setPosition(sf::Vector2f{static_cast<float>(mapPosition.x * tileWidth * 4), static_cast<float>(mapPosition.y * tileHeight * 4)});
    tileMap[mapPosition] = targetTile;
}