#include "Game.h"
#include "ResourceManager.h"
#include "../entities/Player.h"
#include "../entities/Platform.h"
#include "../world/Map.h"
#include "../graphics/Animation.h"
#include "../graphics/TextureInfo.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Game::Game() 
    : window(sf::VideoMode({512, 512}), "SFML TEST"),
      view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_HEIGHT, VIEW_HEIGHT)),
    deltaTime(0.0f),
    frameCounter(0),
    currentFps(0.0f),
    windowTitle("SFML TEST") {
}

Game::~Game() {
}

void Game::run() {
    initialize();
    
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();
        ++frameCounter;
        updateFpsDisplay();
        
        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::initialize() {
    window.setFramerateLimit(GameConstants::TARGET_FPS);

    auto& resourceManager = ResourceManager::getInstance();
    
    // Load player textures
    if (!resourceManager.loadTexture("player_idle", std::filesystem::path("Hiker") / "Separated Animation PNGs" / "Idle.png") ||
        !resourceManager.loadTexture("player_walk", std::filesystem::path("Hiker") / "Separated Animation PNGs" / "Walk.png") ||
        !resourceManager.loadTexture("player_jump", std::filesystem::path("Hiker") / "Separated Animation PNGs" / "Jump.png") ||
        !resourceManager.loadTexture("player_run", std::filesystem::path("Hiker") / "Separated Animation PNGs" / "Run.png") ||
        !resourceManager.loadTexture("player_flying", std::filesystem::path("Hiker") / "Separated Animation PNGs" / "Flying.png")) {
        std::cerr << "Error loading player textures" << std::endl;
        return;
    }
    
    // Create animations
    Animation idleAnimation(TextureInfo{ 
        resourceManager.getTexture("player_idle"), 
        sf::Vector2u(6, 1), 0.3f, sf::Vector2i(8, 8), sf::Vector2i(16, 24) 
    });
    Animation walkAnimation(TextureInfo{ 
        resourceManager.getTexture("player_walk"), 
        sf::Vector2u(8, 1), 0.3f, sf::Vector2i(8, 8), sf::Vector2i(16, 24) 
    });
    Animation jumpAnimation(TextureInfo{ 
        resourceManager.getTexture("player_jump"), 
        sf::Vector2u(3, 1), 0.3f, sf::Vector2i(8, 8), sf::Vector2i(16, 24) 
    });
    Animation runAnimation(TextureInfo{ 
        resourceManager.getTexture("player_run"), 
        sf::Vector2u(6, 1), 0.3f, sf::Vector2i(8, 8), sf::Vector2i(16, 24) 
    });
    Animation flyingAnimation(TextureInfo{ 
        resourceManager.getTexture("player_flying"), 
        sf::Vector2u(2, 1), 0.3f, sf::Vector2i(8, 8), sf::Vector2i(16, 24) 
    });
    
    std::unordered_map<Player::State, Animation> stateAnimationMap = {
        {Player::State::Idle, idleAnimation},
        {Player::State::Walk, walkAnimation},
        {Player::State::Jump, jumpAnimation},
        {Player::State::Run, runAnimation},
        {Player::State::Flying, flyingAnimation}
    };
    
    // Create game objects
    player = std::make_unique<Player>(stateAnimationMap, 32.0f);
    
    // Load map
    map = std::make_unique<Map>();
    std::filesystem::path mapLocation = resourceManager.getBasePath() / "maps" / "rich_map.json";
    map->Initialize(mapLocation.string());
    
    // Create platforms
    platform1 = std::make_unique<Platform>(nullptr, sf::Vector2f(400.0f, 200.0f), sf::Vector2f(500.0f, 200.0f));
    platform2 = std::make_unique<Platform>(nullptr, sf::Vector2f(400.0f, 200.0f), sf::Vector2f(500.0f, 0.0f));
}

void Game::handleEvents() {
    const auto onClose = [this](const sf::Event::Closed&) {
        window.close();
    };
    
    const auto onKeyPressed = [this](const sf::Event::KeyPressed& keyPressed) {
        if (keyPressed.scancode == sf::Keyboard::Scancode::Escape)
            window.close();
        else if (keyPressed.scancode == sf::Keyboard::Scancode::C && keyPressed.control) {
            std::cout << "Ctrl+C pressed" << std::endl;
        }
    };
    
    const auto onWindowResized = [this](const sf::Event::Resized& resized) {
        std::cout << "New window size: " << resized.size.x << "x" << resized.size.y << std::endl;
        resizeView(window, view);
    };
    
    const auto onTextEntered = [](const sf::Event::TextEntered& textEntered) {
        if (textEntered.unicode < 128) {
            std::cout << static_cast<char>(textEntered.unicode);
        }
    };
    
    window.handleEvents(onClose, onKeyPressed, onWindowResized, onTextEntered);
}

void Game::update(float deltaTime) {
    player->Update(deltaTime);
    map->Update(deltaTime);
    
    auto playerCollider = player->GetCollider();
    platform1->GetCollider().CheckCollision(playerCollider, 0.0f);
    platform2->GetCollider().CheckCollision(playerCollider, 1.0f);
    
    view.setCenter(player->GetPosition());
}

void Game::render() {
    window.clear();
    window.setView(view);
    map->Draw(window);
    player->Draw(window);
    platform1->Draw(window);
    platform2->Draw(window);
    window.display();
}

void Game::updateFpsDisplay() {
    const float elapsed = fpsClock.getElapsedTime().asSeconds();
    if (elapsed < 0.5f) {
        return;
    }

    currentFps = static_cast<float>(frameCounter) / elapsed;
    frameCounter = 0;
    fpsClock.restart();

    std::ostringstream titleBuilder;
    titleBuilder << windowTitle << " | FPS: "
                 << std::fixed << std::setprecision(1) << currentFps
                 << " / " << GameConstants::TARGET_FPS;
    window.setTitle(titleBuilder.str());
}

void Game::resizeView(const sf::RenderWindow& window, sf::View& view) {
    float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(sf::Vector2f{VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT});
}
