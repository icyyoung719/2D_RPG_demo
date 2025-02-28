#pragma once
#include <vector>
#include <map>
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
    
    Player(std::map<State, Animation> &animations, float speed);
    ~Player();

    void Update(float deltaTime) override;
    void Draw(sf::RenderWindow& window) override;

private:
    // 每个State对应一个Animation
    std::map<State, Animation> animations;
    float speed;
    bool faceRight;
    State state;
};