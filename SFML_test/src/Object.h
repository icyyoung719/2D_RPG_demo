#pragma once
#include <SFML/Graphics.hpp>

#include "Collider.h"

class Object
{
public:
    virtual ~Object() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Draw(sf::RenderWindow& window) {
        window.draw(sprite);        
    }


    void SetPosition(sf::Vector2f position) { body.setPosition(position); }
    sf::Vector2f GetPosition() const { return body.getPosition(); }
    Collider GetCollider() { return Collider(body); }

protected:
    sf::RectangleShape body;
    sf::Sprite sprite;
    sf::Texture texture;
};
