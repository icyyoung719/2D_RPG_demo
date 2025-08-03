#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <string>
#include "Tile.h"
#include "MapLoader.h"

namespace std {
	template <>
	struct hash<sf::Vector2i>
	{
	std::size_t operator()(const sf::Vector2i& v) const noexcept
	{
	return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
	}
	};
}

class Map
{
public:
	Map();
	~Map();

	// 初始化地图（使用旧格式）
	void Initialize(const std::string& jsonPath);
	// 加载地图（使用旧格式）
	void LoadMap(const std::string& jsonPath);
	// 加载Tiled格式地图
	void LoadTiledMap(const std::string& jsonPath);
	// 更新地图
	void Update(float deltaTime);
	// 绘制地图
	void Draw(sf::RenderWindow& window);
	// 辅助方法：添加tile到地图
	void addTile(sf::Vector2i mapPosition, Tile tile);

private:
	int tileWidth;
	int tileHeight;
	int totalTilesX;
	int totalTilesY;

	sf::Texture tileSheetTexture;

	// 存储地图中的tile
	std::unordered_map<sf::Vector2i, Tile> tileMap;

	// 地图加载器
	MapLoader mapLoader;
};


/*
	对于类似于背景的静态团，我们只保留一个全局的Sprite，因为他们不会改变，

	对于如箱子、矿石等，可能被单独改变状态的物体，我们需要对每个对象都创造一个Tile(包含Sprite),并注意编写Update函数

	算了，暂时我们还是将所有的都创建一个Tile吧，这样可以方便的对每个Tile进行操作，而且地图中也不会有太多的Tile

	后续可以优化
*/