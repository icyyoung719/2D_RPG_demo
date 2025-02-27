#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <optional>
#include <iostream>

Game::Game()
    : mWindow(sf::VideoMode({ 800, 600 }), "Snake Game")
    , mDirection(Right)
    , mIsMoving(false)
    , mIsGameOver(false)
{
    mSnakeSegment.setSize(sf::Vector2f(20, 20));
    mSnakeSegment.setFillColor(sf::Color::Green);

    mFood.setSize(sf::Vector2f(20, 20));
    mFood.setFillColor(sf::Color::Red);

    // 初始化蛇
    mSnake.push_back(SnakeSegment(5, 5));
    mSnake.push_back(SnakeSegment(4, 5));
    mSnake.push_back(SnakeSegment(3, 5));

    // 随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 生成第一个食物
    spawnFood();
}

void Game::run()
{
    sf::Clock clock;
    while (mWindow.isOpen())
    {
        processEvents();
        if (!mIsGameOver)
        {
            sf::Time deltaTime = clock.restart();
            if (deltaTime.asSeconds() > 0.1f)
            {
                update();
            }
        }
        else {
            std::cout << "Game Over!" << std::endl;
            mWindow.close();
        }
        render();
    }
}

void Game::processEvents()
{
    while (const auto event = mWindow.pollEvent())
    {
        // 处理关闭事件
        if (event->is<sf::Event::Closed>())
        {
            mWindow.close();
        }

        // 处理按键按下事件
        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
        {
            handlePlayerInput(keyEvent->code, true);
        }

        // 可选：处理按键释放事件
        if (const auto* keyEvent = event->getIf<sf::Event::KeyReleased>())
        {
            handlePlayerInput(keyEvent->code, false);
        }
    }
}

void Game::update()
{
    moveSnake();
    checkCollisions();
}

void Game::render()
{
    mWindow.clear();

    for (const auto& segment : mSnake)
    {
        mSnakeSegment.setPosition(sf::Vector2<float>(float(segment.x * 20), float(segment.y * 20)));
        mWindow.draw(mSnakeSegment);
    }

    mFood.setPosition(sf::Vector2<float>(float(mFoodPosition.x * 20), float(mFoodPosition.y * 20)));
    mWindow.draw(mFood);

    mWindow.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
    if (isPressed)
    {
        if (key == sf::Keyboard::Key::Up && mDirection != Down)
            mDirection = Up;
        else if (key == sf::Keyboard::Key::Down && mDirection != Up)
            mDirection = Down;
        else if (key == sf::Keyboard::Key::Left && mDirection != Right)
            mDirection = Left;
        else if (key == sf::Keyboard::Key::Right && mDirection != Left)
            mDirection = Right;
    }
}

void Game::moveSnake()
{
    SnakeSegment newHead = mSnake.front();
    switch (mDirection)
    {
    case Up:    --newHead.y; break;
    case Down:  ++newHead.y; break;
    case Left:  --newHead.x; break;
    case Right: ++newHead.x; break;
    }

    mSnake.insert(mSnake.begin(), newHead);

    if (newHead.x == mFoodPosition.x && newHead.y == mFoodPosition.y)
    {
        spawnFood();
    }
    else
    {
        mSnake.pop_back();
    }

    mIsMoving = false; // Reset mIsMoving after moving the snake
}

void Game::checkCollisions()
{
    SnakeSegment head = mSnake.front();

    // 检查是否与自身碰撞
    for (size_t i = 1; i < mSnake.size(); ++i)
    {
        if (mSnake[i].x == head.x && mSnake[i].y == head.y)
        {
            mIsGameOver = true;
        }
    }

    // 检查是否与边界碰撞
    if (head.x < 0 || head.x >= 40 || head.y < 0 || head.y >= 30)
    {
        mIsGameOver = true;
    }
}

void Game::spawnFood()
{
    mFoodPosition.x = std::rand() % 40;
    mFoodPosition.y = std::rand() % 30;
}