// #pragma once
// #include <SFML/Graphics.hpp>
// #include <string>
// #include <vector>
// #include <memory>
// #include "dependency/JsonHandler.h"
// #include "TilesetLoader.h"

// class Map;

// struct MapLayer {
// 	std::string name;          // 图层名称
// 	int width;                 // 图层宽度
// 	int height;                // 图层高度
// 	std::vector<int> data;     // 图层数据(tile的全局ID)
// 	float opacity;             // 不透明度
// 	bool visible;              // 是否可见
// 	int x;                     // x坐标
// 	int y;                     // y坐标
// };

// class MapLoader {
// public:
// 	MapLoader() = default;
// 	~MapLoader();

// 	// 加载map JSON文件
// 	bool loadFromFile(const std::string& jsonPath, Map& map);

// 	// 获取图层信息
// 	const std::vector<MapLayer>& getLayers() const { return layers; }

// 	// 获取地图方向
// 	const std::string& getOrientation() const { return orientation; }

// 	// 获取地图宽度(以tile为单位)
// 	int getMapWidth() const { return mapWidth; }

// 	// 获取地图高度(以tile为单位)
// 	int getMapHeight() const { return mapHeight; }

// private:
// 	std::vector<MapLayer> layers;          // 图层数组
// 	std::vector<TilesetLoader> tilesets;   // tileset加载器数组
// 	std::string orientation;               // 地图方向
// 	int mapWidth;                          // 地图宽度(以tile为单位)
// 	int mapHeight;                         // 地图高度(以tile为单位)
// 	int tileWidth;                         // 单个tile宽度
// 	int tileHeight;                        // 单个tile高度

// 	// 解析JSON数据
// 	bool parseJson(const json::JsonHandler& jsonHandler);

// 	// 加载tileset
// 	bool loadTileset(const std::string& sourcePath, TilesetLoader& tilesetLoader);

// 	// 从图层数据创建tile
// 	void createTilesFromLayer(const MapLayer& layer, Map& map);

// 	// 通过全局ID获取对应的tileset
// 	TilesetLoader* getTilesetByGid(int gid);

// 	// 获取tile在tileset中的位置
// 	sf::Vector2i getTileSheetPosition(int gid, TilesetLoader* tilesetLoader);
// };