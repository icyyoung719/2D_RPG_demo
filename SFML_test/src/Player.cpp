#include "Player.h"

Player::Player(std::map<State, Animation> &animations, float speed):animations(animations), speed(speed)
{
    state = State::Idle;
    faceRight = true;

    body.setOrigin(body.getSize() / 2.0f);
    body.setSize(sf::Vector2f(96.0f, 96.0f));
    body.setPosition({100.0f, 100.0f});
    body.setTexture(animations[state].getTexture());
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
    body.setTextureRect(animations[state].uvRect);
    body.move(movement);
}

void Player::Draw(sf::RenderWindow &window)
{
    window.draw(body);
}