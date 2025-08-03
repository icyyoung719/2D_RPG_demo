#include "MapLoader.h"
#include <iostream>
#include <filesystem>
#include "Tile.h"


MapLoader::~MapLoader() {
}

bool MapLoader::loadFromFile(const std::string& jsonPath, Map& map) {
    // 加载JSON文件
    json::JsonHandler jsonHandler(jsonPath);
    if (!parseJson(jsonHandler)) {
        return false;
    }

    // 从图层数据创建tile
    for (const auto& layer : layers) {
        if (layer.visible) {
            createTilesFromLayer(layer, map);
        }
    }

    return true;
}

bool MapLoader::parseJson(const json::JsonHandler& jsonHandler) {
    // 获取根元素
    json::JsonElement* root = jsonHandler.getJsonElement("");
    if (!root || root->getType() != json::JsonElement::Type::JSON_OBJECT) {
        std::cerr << "Invalid JSON structure" << std::endl;
        return false;
    }

    json::JsonObject* jsonObject = root->asObject();
    if (!jsonObject) {
        std::cerr << "Failed to get JSON object from root element" << std::endl;
        return false;
    }

    // 解析基本信息
    mapWidth = jsonObject->at("width")->asInt();
    mapHeight = jsonObject->at("height")->asInt();
    tileWidth = jsonObject->at("tilewidth")->asInt();
    tileHeight = jsonObject->at("tileheight")->asInt();
    orientation = jsonObject->at("orientation")->asString();

    // 解析tilesets数组
    json::JsonElement* tilesetsElement = jsonObject->at("tilesets").get();
    if (tilesetsElement && tilesetsElement->getType() == json::JsonElement::Type::JSON_ARRAY) {
        json::JsonArray* tilesetsArray = tilesetsElement->asArray();
        if (tilesetsArray) {
            for (const auto& tilesetElement : *tilesetsArray) {
                if (tilesetElement && tilesetElement->getType() == json::JsonElement::Type::JSON_OBJECT) {
                    json::JsonObject* tilesetObject = tilesetElement->asObject();
                    if (tilesetObject) {
                        std::string sourcePath = tilesetObject->at("source")->asString();
                        TilesetLoader tilesetLoader;
                        if (loadTileset(sourcePath, tilesetLoader)) {
                            tilesets.push_back(tilesetLoader);
                        }
                    }
                }
            }
        }
    }

    // 解析layers数组
    json::JsonElement* layersElement = jsonObject->at("layers").get();
    if (layersElement && layersElement->getType() == json::JsonElement::Type::JSON_ARRAY) {
        json::JsonArray* layersArray = layersElement->asArray();
        if (layersArray) {
            for (const auto& layerElement : *layersArray) {
                if (layerElement && layerElement->getType() == json::JsonElement::Type::JSON_OBJECT) {
                    json::JsonObject* layerObject = layerElement->asObject();
                    if (layerObject) {
                        // 只处理tilelayer类型的图层
                        if (layerObject->at("type")->asString() != "tilelayer") {
                            continue;
                        }

                        MapLayer layer;
                        layer.name = layerObject->at("name")->asString();
                        layer.width = layerObject->at("width")->asInt();
                        layer.height = layerObject->at("height")->asInt();
                        layer.opacity = static_cast<float>(layerObject->at("opacity")->asDouble());
                        layer.visible = layerObject->at("visible")->asBool();
                        layer.x = layerObject->at("x")->asInt();
                        layer.y = layerObject->at("y")->asInt();

                        // 解析data数组
                        json::JsonElement* dataElement = layerObject->at("data").get();
                        if (dataElement && dataElement->getType() == json::JsonElement::Type::JSON_ARRAY) {
                            json::JsonArray* dataArray = dataElement->asArray();
                            if (dataArray) {
                                for (const auto& dataElement : *dataArray) {
                                    layer.data.push_back(dataElement->asInt());
                                }
                            }
                        }

                        layers.push_back(layer);
                    }
                }
            }
        }
    }

    return true;
}

bool MapLoader::loadTileset(const std::string& sourcePath, TilesetLoader& tilesetLoader) {
    std::cout << "Loading tileset: " << sourcePath << std::endl;
    return tilesetLoader.loadFromFile(sourcePath);
}

void MapLoader::createTilesFromLayer(const MapLayer& layer, Map& map) {
    for (int y = 0; y < layer.height; y++) {
        for (int x = 0; x < layer.width; x++) {
            int index = y * layer.width + x;
            int gid = layer.data[index];

            // gid为0表示空tile
            if (gid == 0) {
                continue;
            }

            // 获取对应的tileset
            TilesetLoader* tilesetLoader = getTilesetByGid(gid);
            if (!tilesetLoader) {
                std::cerr << "No tileset found for gid: " << gid << std::endl;
                continue;
            }

            // 获取tile在tileset中的位置
            sf::Vector2i sheetPosition = getTileSheetPosition(gid, tilesetLoader);

            // 检查是否有动画
            bool isDynamic = false;
            float switchTime = 0.0f;
            std::vector<sf::Vector2i> updateLoop;

            // 获取tileset数据
            const TilesetData tilesetData = tilesetLoader->data;

            // 查找对应的tile信息
            for (const auto& tile : tilesetData.tiles) {
                int localId = gid - 1; // 假设firstgid为1
                if (tile.id == localId) {
                    if (!tile.animation.empty()) {
                        isDynamic = true;
                        switchTime = static_cast<float>(tile.animation[0].duration) / 1000.0f; // 转换为秒

                        // 创建动画循环
                        for (const auto& frame : tile.animation) {
                            sf::Vector2i framePosition = getTileSheetPosition(frame.tileid + 1, tilesetLoader); // 假设firstgid为1
                            updateLoop.push_back(framePosition);
                        }
                    }
                    break;
                }
            }

            // 创建tile
            Tile newTile(const_cast<sf::Texture*>(&tilesetLoader->getTexture()), sheetPosition, isDynamic, switchTime);

            // 设置动画循环
            if (isDynamic && !updateLoop.empty()) {
                newTile.setUpdateLoop(updateLoop);
            }

            // 添加到地图
            map.addTile(sf::Vector2i(x, y), newTile);
        }
    }
}

TilesetLoader* MapLoader::getTilesetByGid(int gid) {
    // 找到第一个firstgid小于等于gid的tileset
    // 注意：这里简化处理，假设firstgid为1
    if (!tilesets.empty()) {
        return &tilesets[0];
    }
    return nullptr;
}

sf::Vector2i MapLoader::getTileSheetPosition(int gid, TilesetLoader* tilesetLoader) {
    // 计算tile在tileset中的索引
    int localId = gid - 1; // 假设firstgid为1

    // 获取tileset数据
    const TilesetData tilesetData = tilesetLoader->data;

    // 计算行列
    int tilesPerRow = tilesetData.columns;
    int row = localId / tilesPerRow;
    int col = localId % tilesPerRow;

    return sf::Vector2i(col, row);
}