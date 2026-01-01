#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

// Forward declarations
class Player;
class Map;
class Platform;

class Game {
public:
    Game();
    ~Game();

    // Disable copy and move
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    void run();

private:
    void initialize();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void resizeView(const sf::RenderWindow& window, sf::View& view);

    // Window and view
    static constexpr float VIEW_HEIGHT = 512.0f;
    sf::RenderWindow window;
    sf::View view;

    // Game objects
    std::unique_ptr<Player> player;
    std::unique_ptr<Map> map;
    std::unique_ptr<Platform> platform1;
    std::unique_ptr<Platform> platform2;

    // Timing
    sf::Clock clock;
    float deltaTime;
};
