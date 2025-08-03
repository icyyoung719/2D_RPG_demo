#pragma once
#include <SFML/Graphics.hpp>

class Map
{
public:
    Map();
    ~Map();

    void Draw(sf::RenderWindow* window);
    void Update(double deltaTime);
    void Initialize();
    void Load();

private:
};