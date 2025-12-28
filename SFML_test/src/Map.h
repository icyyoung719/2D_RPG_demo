#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Tileset.h"

struct MapLayer {
    inline int getTileId(int x, int y) const {
        if (x < 0 || y < 0 || x >= width || y >= height) return 0;
        return data[y * width + x];
    }

	int id = -1;
	std::string name;
    int width;
    int height;
    std::vector<int> data;  // GID map, row-major: data[y * width + x]
    int opacity = 1;
    bool visible = true;
	int x = 0;
	int y = 0;
};

struct TilesetReference {
    int firstgid;           // First global tile ID in this tileset
    std::string source;     // Path to the tileset JSON file
};

class Map {
public:
    Map() = default;
    Map(const std::string& filename);
	~Map() = default;
    Map(const Map&) = delete;
	Map& operator=(Map&& other) = delete;
	Map(Map&& other) = delete;

	void Draw(sf::RenderWindow& window);
	void Update(double deltaTime);
	void Initialize(const std::string& filename);
    void Load(const std::string& filename);

    /// 获取图层（可选）
    // std::optional<const MapLayer*> getLayer(const std::string& name) const;

    /// 获取 tile ID（GID），若图层或坐标非法，返回 0
    // int getTileId(int x, int y, const std::string& layerName) const;

    /// 判断某 tile 是否在地图边界内
    // bool isInBounds(int x, int y) const;
private:
    // Helper functions
    Tileset* getTilesetByGid(int gid);
    sf::Vector2i getTilePositionInTileset(int gid, const Tileset* tileset, int firstgid);
    
    int mapWidth = 0;     // in tiles
    int mapHeight = 0;
    int tileWidth = 0;    // in pixels
    int tileHeight = 0;
    std::string orientation = "orthogonal";
    std::string renderOrder = "right-down";
    bool infinite = false;

	std::vector<MapLayer> layers;
    std::vector<TilesetReference> tilesetRefs;
    std::vector<Tileset> tilesets;
    std::vector<std::vector<Tile>> tiles;  // 2D array of tiles for rendering
};
