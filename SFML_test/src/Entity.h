#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

class Entity
{
public:
	virtual ~Entity() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;

	sf::Vector2f GetPosition() const { return body.getPosition(); }
	Collider GetCollider() { return Collider(body); }

protected:
	sf::RectangleShape body;
};