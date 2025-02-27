#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>

enum Direction { Up, Down, Left, Right };

struct SnakeSegment
{
    int x, y;
    SnakeSegment(int x, int y) : x(x), y(y) {}
};

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    void moveSnake();
    void checkCollisions();
    void spawnFood();

    sf::RenderWindow mWindow;
    sf::RectangleShape mSnakeSegment;
    sf::RectangleShape mFood;
    std::vector<SnakeSegment> mSnake;
    Direction mDirection;
    bool mIsMoving;
    bool mIsGameOver;
    sf::Vector2i mFoodPosition;
};

#endif // GAME_H