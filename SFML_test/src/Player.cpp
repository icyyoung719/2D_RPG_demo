#include "Player.h"

Player::Player(std::unordered_map<State, Animation> &animations, float speed):animations(animations), speed(speed)
{
    state = State::Idle;
    faceRight = true;
    
    // resize to 4 times larger
    sf::Vector2f actualSize(16.0f*4, 24.0f*4);
    this->speed = speed * 4.0f;

    body.setSize(actualSize);
    // body.setScale(sf::Vector2f{4.0f,4.0f});
    //auto texture = animations[state].getTexture();
    body.setTexture(animations[state].getTexture());
    body.setOrigin(actualSize / 2.0f);
    body.setPosition({100.0f, 100.0f});
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
    sf::Vector2f movement(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= speed * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += speed * deltaTime;

    if(movement.x == 0.0f)
        state = State::Idle;
    else
    {
        state = State::Walk;

        if(movement.x > 0.0f)
            faceRight = true;
        else
            faceRight = false;
    }
    animations[state].Update(0, deltaTime, faceRight);
    body.setTexture(animations[state].getTexture());
    body.setTextureRect(animations[state].textureRealUvRect);
    body.move(movement);
}

void Player::Draw(sf::RenderWindow &window)
{
    window.draw(body);
}