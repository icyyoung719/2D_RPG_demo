#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "../dependency/JsonHandler.h"

struct TilesetAnimationFrame {
	int tileid;     // 帧对应的tile ID
	int duration;   // 帧持续时间(毫秒)
};

struct TilesetTile {	// 动态tile的信息
	int id;                                 // tile ID
	std::vector<TilesetAnimationFrame> animation; // 动画帧
};

// 直接在tileset的json中存储的数据信息
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
	// 静态tile不存储，直接使用id或位置获取
	std::vector<TilesetTile> tiles; // tile数组
};

class Tileset {
public:
	Tileset() {};
	~Tileset() = default;

	bool loadFromFile(const std::string& jsonPath);
	bool contains(int pos) const;

	// 获取纹理
	const sf::Texture& getTexture() const { return texture; }

	// 获取指定id的tile，id合法性由外部保证
	const TilesetTile& getTile(int id) const { return data.tiles[id]; }

public:
	TilesetData data;  // 数据实例，后续需要考虑访问权限问题，暂时用public
private:
	sf::Texture texture;

	bool parseData(const json::JsonObject* jsonObject);
};