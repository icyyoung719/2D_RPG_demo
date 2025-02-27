#pragma once
#include <vector>
#include <map>

#include <SFML\Graphics.hpp>

#include "Animation.h"

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
    
    //Player(std::map<State, sf::Texture*> &stateTextureMap, sf::Vector2u imageCount, float switchTime, float speed);
    Player(std::map<State, Animation> &animations, float speed);
    ~Player();

    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
    sf::Vector2f GetPosition() { return body.getPosition(); }
private:
    sf::RectangleShape body;
    // 每个State对应一个Animation
    std::map<State, Animation> animations;
    float speed;
    bool faceRight;
    State state;
};