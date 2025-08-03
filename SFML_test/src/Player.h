#pragma once
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Collider.h"
#include "Entity.h"

class Player : public Entity
{
public:
	enum class State
	{
	Idle,
	Walk,
	Jump,
	Run,
	Flying
	};
	
	Player(std::unordered_map<State, Animation> &animations, float speed);
	~Player();

	void Update(float deltaTime) override;
	void Draw(sf::RenderWindow& window) override;

private:
	// every state has its own animation
	std::unordered_map<State, Animation> animations;
	float speed;
	bool faceRight;
	State state;
};