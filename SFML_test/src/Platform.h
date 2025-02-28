#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"
#include "Entity.h"

class Platform : public Entity
{
public:
    Platform(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position);
    ~Platform();

    void Update(float deltaTime) override {}
    void Draw(sf::RenderWindow& window) override;

private:
};