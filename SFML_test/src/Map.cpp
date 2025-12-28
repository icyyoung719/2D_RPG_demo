#include "Map.h"

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <cassert>

#include "../dependency/JsonHandler.h"

Map::Map(const std::string& filename)
{
	Load(filename);
}

void Map::Initialize(const std::string& filename)
{
	Load(filename);
}

void Map::Load(const std::string& filename)
{
	json::JsonHandler jsonHandler(filename);

	json::JsonElement* root = jsonHandler.getJsonElement("");
	if (!root || root->getType() != json::JsonElement::Type::JSON_OBJECT)
	{
		assert(false);
	}

	json::JsonObject* map = root->asObject();

	// 初始化Map属性
	mapWidth = map->at("width")->asInt();
	mapHeight = map->at("height")->asInt();
	tileWidth = map->at("tilewidth")->asInt();
	tileHeight = map->at("tileheight")->asInt();
	infinite = map->at("infinite")->asBool();
	orientation = map->at("orientation")->asString();
	renderOrder = map->at("renderorder")->asString();
	
	// 加载Tileset引用
	json::JsonArray* tilesetArray = map->at("tilesets")->asArray();
	if (tilesetArray)
	{
		std::filesystem::path mapDir = std::filesystem::absolute(filename).parent_path();
		for (size_t i = 0; i < tilesetArray->size(); i++)
		{
			json::JsonObject* tilesetObj = tilesetArray->at(i)->asObject();
			TilesetReference ref;
			ref.firstgid = tilesetObj->at("firstgid")->asInt();
			ref.source = tilesetObj->at("source")->asString();
			tilesetRefs.push_back(ref);
			
			// 加载实际的tileset
			std::filesystem::path tilesetPath = mapDir / ref.source;
			Tileset tileset;
			if (tileset.loadFromFile(tilesetPath.string()))
			{
				tilesets.push_back(std::move(tileset));
				std::cout << "Loaded tileset: " << tilesetPath << " with firstgid: " << ref.firstgid << std::endl;
			}
			else
			{
				std::cerr << "Failed to load tileset: " << tilesetPath << std::endl;
			}
		}
	}
	
	// 初始化Layer属性
	json::JsonArray* layerArray = map->at("layers")->asArray();
	MapLayer layer;
	if (!layerArray)
		assert(false);
	for (int i = 0; i < layerArray->size(); i++)
	{
		json::JsonObject* layerObject = layerArray->at(i)->asObject();
		layer.id = layerObject->at("id")->asInt();
		layer.name = layerObject->at("name")->asString();
		layer.width = layerObject->at("width")->asInt();
		layer.height = layerObject->at("height")->asInt();
		layer.opacity = layerObject->at("opacity")->asInt();
		layer.visible = layerObject->at("visible")->asBool();
		layer.x = layerObject->at("x")->asInt();
		layer.y = layerObject->at("y")->asInt();

		std::vector<int> data;
		json::JsonArray* dataArray = layerObject->at("data")->asArray();
		for (int j = 0; j < dataArray->size(); j++)
		{
			data.push_back(dataArray->at(j)->asInt());
		}
		layer.data = std::move(data);

		layers.push_back(layer);
	}
	
	// 创建tile数组用于渲染
	tiles.resize(mapHeight);
	for (int y = 0; y < mapHeight; y++)
	{
		tiles[y].resize(mapWidth);
	}
	
	// 从layers创建tiles
	for (const auto& layer : layers)
	{
		if (!layer.visible) continue;
		
		for (int y = 0; y < layer.height && y < mapHeight; y++)
		{
			for (int x = 0; x < layer.width && x < mapWidth; x++)
			{
				int gid = layer.getTileId(x, y);
				if (gid == 0) continue;  // 空tile
				
				Tileset* tileset = getTilesetByGid(gid);
				if (!tileset) continue;
				
				// 获取该tileset的firstgid
				int firstgid = 0;
				for (size_t i = 0; i < tilesets.size(); i++)
				{
					if (&tilesets[i] == tileset)
					{
						firstgid = tilesetRefs[i].firstgid;
						break;
					}
				}
				
				sf::Vector2i tilePos = getTilePositionInTileset(gid, tileset, firstgid);
				
				// 检查是否有动画
				bool isDynamic = false;
				float switchTime = 0.0f;
				std::vector<sf::Vector2i> updateLoop;
				
				int localId = gid - firstgid;
				for (const auto& tileData : tileset->data.tiles)
				{
					if (tileData.id == localId && !tileData.animation.empty())
					{
						isDynamic = true;
						switchTime = static_cast<float>(tileData.animation[0].duration) / 1000.0f;
						
						// 创建动画循环
						for (const auto& frame : tileData.animation)
						{
							sf::Vector2i framePos = getTilePositionInTileset(frame.tileid + firstgid, tileset, firstgid);
							updateLoop.push_back(framePos);
						}
						break;
					}
				}
				
				// 创建tile - 使用const_cast获取非const纹理指针
				tiles[y][x] = Tile(const_cast<sf::Texture*>(&tileset->getTexture()), tilePos, isDynamic, switchTime);
				if (isDynamic && !updateLoop.empty())
				{
					tiles[y][x].setUpdateLoop(updateLoop);
				}
			}
		}
	}
}

void Map::Draw(sf::RenderWindow& window)
{
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			if (tiles[y][x].sprite)
			{
				sf::Vector2f position(x * tileWidth * GameConstants::TILE_SCALE, y * tileHeight * GameConstants::TILE_SCALE);
				tiles[y][x].sprite->setPosition(position);
				window.draw(*tiles[y][x].sprite);
			}
		}
	}
}

void Map::Update(double deltaTime)
{
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			tiles[y][x].Update(static_cast<float>(deltaTime));
		}
	}
}

Tileset* Map::getTilesetByGid(int gid)
{
	// 找到合适的tileset - 需要找到firstgid <= gid且最大的那个
	Tileset* result = nullptr;
	int maxFirstgid = 0;
	
	for (size_t i = 0; i < tilesets.size(); i++)
	{
		int firstgid = tilesetRefs[i].firstgid;
		if (firstgid <= gid && firstgid > maxFirstgid)
		{
			maxFirstgid = firstgid;
			result = &tilesets[i];
		}
	}
	
	return result;
}

sf::Vector2i Map::getTilePositionInTileset(int gid, const Tileset* tileset, int firstgid)
{
	int localId = gid - firstgid;
	int columns = tileset->data.columns;
	
	int row = localId / columns;
	int col = localId % columns;
	
	return sf::Vector2i(col, row);
}