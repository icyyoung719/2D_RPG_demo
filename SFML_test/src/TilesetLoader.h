#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "../dependency/JsonHandler.h"

struct TilesetAnimationFrame {
	int tileid;     // 帧对应的tile ID
	int duration;   // 帧持续时间(毫秒)
};

struct TilesetTile {
	int id;                                 // tile ID
	std::vector<TilesetAnimationFrame> animation; // 动画帧
};

struct TilesetData {
	TilesetData() = default;
	TilesetData(const json::JsonObject* jsonObject);
	~TilesetData() = default;
	TilesetData(const TilesetData& other) = default;
	TilesetData(TilesetData&& other) = default;
	TilesetData& operator=(const TilesetData& other) = default;
	TilesetData& operator=(TilesetData&& other) = default;

	int columns;              // tileset中每行的tile数量
	std::string imagePath;    // 纹理图片路径
	int imageHeight;          // 图片高度
	int imageWidth;           // 图片宽度
	int margin;               // 边距
	std::string name;         // tileset名称
	int spacing;              // 间距
	int tileCount;            // tile总数
	int tileHeight;           // 单个tile高度
	int tileWidth;            // 单个tile宽度
	std::vector<TilesetTile> tiles; // tile数组
};

class TilesetLoader {
public:
	TilesetLoader() {};
	~TilesetLoader() = default;

	bool loadFromFile(const std::string& jsonPath);

	// 获取纹理
	const sf::Texture& getTexture() const { return texture; }

public:
	TilesetData data;  // 数据实例
private:
	sf::Texture texture;                // 纹理

	bool parseData(const json::JsonObject* jsonObject);
};