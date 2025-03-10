#include "Map.h"
#include <iostream>
#include <string>
#include "../dependency/JsonHandler.h"

Map::Map() : 
    tileWidth(16), tileHeight(16), totalTilesX(0), totalTilesY(0), tileMap{}
{
}

Map::~Map()
{
}

void Map::Initialize(const std::string& jsonPath) {
    // relative path
    std::cout << "Loading Map with relative path: " << jsonPath << std::endl;
    // absolute path
    std::cout << "Loading Map with absolute path: " << std::filesystem::absolute(jsonPath) << std::endl; 
    LoadMap(jsonPath);
}

void Map::LoadMap(const std::string& jsonPath) {
    json::JsonHandler jsonHandler(jsonPath);

    // test!!
    // json::JsonElement myObject = json::JsonElement("myObject");
    // std::string path = R"(["doadjiwa"])";
    // jsonHandler.insertJsonElement(path, std::move(myObject));

    json::JsonElement* root = jsonHandler.getJsonElement("");
    if (!root || root->getType() != json::JsonElement::Type::JSON_OBJECT) {
        std::cerr << "Invalid JSON structure in " << jsonPath << std::endl;
        return;
    }

    json::JsonObject* jsonObject = root->asObject();
    if (!jsonObject) {
        std::cerr << "Failed to get JSON object from root element." << std::endl;
        return;
    }

    // Load basic map information
    tileWidth = jsonObject->at("tileWidth")->asInt();
    tileHeight = jsonObject->at("tileHeight")->asInt();
    totalTilesX = jsonObject->at("totalTilesX")->asInt();
    totalTilesY = jsonObject->at("totalTilesY")->asInt();

    std::string tileSheetPath = jsonObject->at("tileSheetPath")->asString();
    if (!tileSheetTexture.loadFromFile(tileSheetPath)) {
        std::cerr << "Error loading texture: " << tileSheetPath << std::endl;
        return;
    }

    std::cout << "World Map Texture Loaded" << std::endl;
    std::cout << "Total Tiles X: " << totalTilesX << std::endl;
    std::cout << "Total Tiles Y: " << totalTilesY << std::endl;

    // Load tiles
    json::JsonElement* tilesElement = jsonObject->at("tiles").get();
    if (!tilesElement || tilesElement->getType() != json::JsonElement::Type::JSON_ARRAY) {
        std::cerr << "Invalid 'tiles' array in JSON." << std::endl;
        return;
    }

    json::JsonArray* tilesArray = tilesElement->asArray();
    if (!tilesArray) {
        std::cerr << "Failed to get JSON array from 'tiles' element." << std::endl;
        return;
    }

    for (const auto& tileElement : *tilesArray) {
        if (!tileElement || tileElement->getType() != json::JsonElement::Type::JSON_OBJECT) {
            std::cerr << "Invalid tile element in JSON." << std::endl;
            continue;
        }

        json::JsonObject* tileObject = tileElement->asObject();
        if (!tileObject) {
            std::cerr << "Failed to get JSON object from tile element." << std::endl;
            continue;
        }

        sf::Vector2i mapPosition = {
            tileObject->at("mapPosition")->asArray()->at(0)->asInt(),
            tileObject->at("mapPosition")->asArray()->at(1)->asInt()
        };

        sf::Vector2i sheetPosition = {
            tileObject->at("sheetPosition")->asArray()->at(0)->asInt(),
            tileObject->at("sheetPosition")->asArray()->at(1)->asInt()
        };

        bool isDynamic = tileObject->at("isDynamic")->asBool();
        float switchTime = isDynamic ? static_cast<float>(tileObject->at("switchTime")->asDouble()) : 0.0f;


        Tile tile(&tileSheetTexture, sheetPosition, isDynamic, switchTime);

        if (isDynamic && tileObject->contains("updateLoop")) {
            json::JsonArray* updateLoopArray = tileObject->at("updateLoop")->asArray();
            if (updateLoopArray) {
                std::vector<sf::Vector2i> updateLoop;
                for (const auto& posElement : *updateLoopArray) {
                    if (posElement->getType() == json::JsonElement::Type::JSON_ARRAY) {
                        json::JsonArray* posArray = posElement->asArray();
                        if (posArray && posArray->size() == 2) {
                            sf::Vector2i pos = {
                                posArray->at(0)->asInt(),
                                posArray->at(1)->asInt()
                            };
                            updateLoop.push_back(pos);
                        }
                    }
                }
                tile.setUpdateLoop(updateLoop);
            }
        }

        addTile(mapPosition, tile);
    }
}
void Map::Update(float deltaTime)
{
    for (auto& tile : tileMap)
    {
        tile.second.Update(deltaTime);
    }
}

void Map::Draw(sf::RenderWindow &window)
{
    for (const auto& tile : tileMap)
    {
        if (tile.second.sprite) {
            window.draw(*tile.second.sprite);
        }
    }
}

void Map::addTile(sf::Vector2i mapPosition, Tile targetTile)
{
    targetTile.sprite->setPosition(sf::Vector2f{static_cast<float>(mapPosition.x * tileWidth * 4), static_cast<float>(mapPosition.y * tileHeight * 4)});
    tileMap[mapPosition] = targetTile;
}