#include "MouseTile.h"
#include <iostream>

MouseTile::MouseTile(const sf::Vector2f tileScale, const sf::Vector2i tileSize, const sf::Vector2f offset):
    tileScale(tileScale), tileSize(tileSize), offset(offset)
{
}

MouseTile::~MouseTile()
{
}

void MouseTile::Initialize()
{
}

void MouseTile::Load()
{
    std::string tilePath = "../../../MapEditor/assests/World/Overworld.png";
    bool loadResult = tileSheet.loadFromFile(tilePath);
    if( !loadResult){
        std::cout << "Failed to load tile sheet!" << std::endl;
        std::cout << "Loading MouseTile with absolute path: " << std::filesystem::absolute(tilePath) << std::endl; 
    }
    tileSprite.emplace(tileSheet);
    tileSprite->setTexture(tileSheet);
    tileSprite->setTextureRect(sf::IntRect{{7*tileSize.x, 5*tileSize.y}, tileSize});
    tileSprite->setScale(tileScale);
}

void MouseTile::Update(float deltaTime, sf::Vector2f mousePos)
{
    // set the tile sprite pos to the according block
    // tileSprite->setPosition(mousePos - sf::Vector2f{8.0f * 4, 8.0f * 4});
    int x = static_cast<int>((mousePos.x - offset.x)/ (tileSize.x * tileScale.x));
    int y = static_cast<int>((mousePos.y - offset.y)/ (tileSize.y * tileScale.y));
    sf::Vector2i blockPos = {
        x,
        y
    };
    
    // multiply by 16(tile size) and 4(scale)
    tileSprite->setPosition(sf::Vector2f{blockPos.x * tileSize.x * tileScale.x, blockPos.y * tileSize.y * tileScale.y} + offset);
    
}

void MouseTile::Draw(sf::RenderWindow& window)
{
    window.draw(tileSprite.value());
}

bool MouseTile::IsMouseClickedOnTile(sf::Vector2f& tilePos) const
{
    bool itsOnGrid = true;
    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && itsOnGrid)
    {
        tilePos = tileSprite->getPosition();
        return true;
    }
    return false;
}
