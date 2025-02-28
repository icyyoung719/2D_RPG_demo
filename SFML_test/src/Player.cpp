#include "Player.h"
#include <cmath>

Player::Player(std::unordered_map<State, Animation> &animations, float speed, float jumpHeight):
animations(animations), speed(speed), jumpHeight(jumpHeight)
{
    state = State::Idle;
    faceRight = true;
    
    // resize to 4 times larger
    sf::Vector2f actualSize(16.0f*4, 24.0f*4);

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
    velocity.x *= 0.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        velocity.x -= speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        velocity.x += speed;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && canJump)
    {
        canJump = false;
        state = State::Jump;
        velocity.y = -sqrtf(2.0f * 981.0f * jumpHeight);
    }

    velocity.y += 981.0f * deltaTime;

    if((velocity.x == 0.0f) && (state != State::Jump)&& (state != State::Flying))
        state = State::Idle;
    else
    {
        if(state == State::Idle)
            state = State::Walk;

        if(velocity.x > 0.0f)
            faceRight = true;
        else
            faceRight = false;
    }
    animations[state].Update(0, deltaTime, faceRight);
    body.setTexture(animations[state].getTexture());
    body.setTextureRect(animations[state].textureRealUvRect);
    body.move(velocity * deltaTime);
}

void Player::Draw(sf::RenderWindow &window)
{
    window.draw(body);
}

void Player::OnCollide(sf::Vector2f direction)
{
    if(direction.x < 0.0f)
    {
        // Colliding on the right
        velocity.x = 0.0f;
    }
    else if(direction.x > 0.0f)
    {
        // Colliding on the left
        velocity.x = 0.0f;
    }
    if(direction.y < 0.0f)
    {
        // Colliding on the bottom
        velocity.y = 0.0f;
        canJump = true;
        state = State::Idle;
    }
    else if(direction.y > 0.0f)
    {
        // Colliding on the top
        velocity.y = 0.0f;
    }
}
