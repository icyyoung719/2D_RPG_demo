#pragma once
#include <SFML/Graphics.hpp>


class Grid
{
public:
    Grid(const sf::Vector2f position,
        const sf::Vector2i cellSize,
        const sf::Vector2i totalCells,
        const sf::Vector2i scale,
        const int lineThickness,
        const sf::Color color);
    ~Grid();

    void Initialize();
    void Load();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

    inline const sf::Vector2f GetPosition() const { return position; }
    inline const sf::Vector2f GetTotalSize() const { return totalSize; }
    inline const sf::Vector2i GetTotalCells() const { return totalCells; }
    inline const sf::Vector2i GetCellSize() const { return cellSize; }
    inline const sf::Vector2i GetScale() const { return scale; }
private:

    sf::RectangleShape* hLine;
    sf::RectangleShape* vLine;

    // x is the num of vertical lines, y is the num of horizontal lines
    sf::Vector2i totalLineNum;
    // times the grid will be enlarged
    sf::Vector2i scale;
    sf::Vector2i totalCells;
    sf::Vector2i cellSize;
    // the size of the grids(added up)
    sf::Vector2f totalSize;
    sf::Vector2f position;

    int lineThickness;
    sf::Color color;
};