#pragma once
#include <SFML/Graphics.hpp>

class MouseTile
{
public:
    MouseTile(const sf::Vector2f tileScale,
        const sf::Vector2i tileSize,
        const sf::Vector2f offset);
    ~MouseTile();

    void Initialize();
    void Load();
    void Update(float deltaTime, sf::Vector2f mousePos);
    void Draw(sf::RenderWindow& window);
    //void SetTile(int x, int y);
    bool IsMouseClickedOnTile(sf::Vector2f& tilePos) const;

private:
    std::optional<sf::Sprite> tileSprite;
    sf::Texture tileSheet;

    sf::Vector2f tileScale;
    sf::Vector2i tileSize;
    sf::Vector2f offset;

    bool isMouseOnGrid;
};