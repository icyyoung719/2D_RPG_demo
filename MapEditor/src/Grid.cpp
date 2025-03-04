#include "Grid.h"

Grid::Grid(const sf::Vector2f position,
    const sf::Vector2i cellSize,
    const sf::Vector2i totalCells,
    const sf::Vector2i scale,
    const int lineThickness,
    const sf::Color color):
    position(position), cellSize(cellSize), totalCells(totalCells), scale(scale), lineThickness(lineThickness), color(color)
{
    totalLineNum = sf::Vector2i{totalCells.x + 1, totalCells.y + 1};

    hLine = new sf::RectangleShape[totalLineNum.y];
    vLine = new sf::RectangleShape[totalLineNum.x];
}

Grid::~Grid()
{
    delete [] hLine;
    delete [] vLine;
}

void Grid::Initialize()
{

    totalSize.x = static_cast<float>(totalCells.x * cellSize.x * scale.x + lineThickness);
    totalSize.y = static_cast<float>(totalCells.y * cellSize.y * scale.y);

    for(auto i = 0; i < totalLineNum.y; i++)
    {
        hLine[i].setSize(sf::Vector2f{totalSize.x, static_cast<float>(lineThickness)});
        hLine[i].setPosition( position + sf::Vector2f{0.0f, static_cast<float>(i * cellSize.x * scale.x)});
        hLine[i].setFillColor(color);
    }
    for(auto i = 0; i < totalLineNum.x; i++)
    {
        vLine[i].setSize(sf::Vector2f{static_cast<float>(lineThickness), totalSize.y});
        vLine[i].setPosition( position + sf::Vector2f{static_cast<float>(i * cellSize.y * scale.y), 0.0f});
        vLine[i].setFillColor(color);
    }
}

void Grid::Load()
{
}

void Grid::Update(float deltaTime)
{
}

void Grid::Draw(sf::RenderWindow &window)
{
    for(auto i = 0; i < totalLineNum.y; i++)
    {
        window.draw(hLine[i]);
    }
    for (auto i = 0; i < totalLineNum.x; i++)
    {
        window.draw(vLine[i]);
    }
}
