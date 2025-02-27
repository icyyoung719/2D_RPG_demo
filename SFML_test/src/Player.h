#pragma once
#include <vector>
#include <map>

#include <SFML\Graphics.hpp>

#include "Animation.h"
#include "Collider.h"

class Player
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
    
    Player(std::map<State, Animation> &animations, float speed);
    ~Player();

    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

    sf::Vector2f GetPosition() { return body.getPosition(); }
    Collider GetCollider() { return Collider(body); }
private:
    sf::RectangleShape body;
    // 每个State对应一个Animation
    std::map<State, Animation> animations;
    float speed;
    bool faceRight;
    State state;
};