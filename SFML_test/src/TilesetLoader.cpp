#include "TilesetLoader.h"
#include <iostream>
#include <filesystem>

// TilesetData构造函数实现
TilesetData::TilesetData(const json::JsonObject* jsonObject) :
	columns(jsonObject->at("columns")->asInt()),
	imagePath(jsonObject->at("image")->asString()),
	imageHeight(jsonObject->at("imageheight")->asInt()),
	imageWidth(jsonObject->at("imagewidth")->asInt()),
	margin(jsonObject->at("margin")->asInt()),
	name(jsonObject->at("name")->asString()),
	spacing(jsonObject->at("spacing")->asInt()),
	tileCount(jsonObject->at("tilecount")->asInt()),
	tileHeight(jsonObject->at("tileheight")->asInt()),
	tileWidth(jsonObject->at("tilewidth")->asInt()) {
	// 解析tiles数组
	json::JsonElement* tilesElement = jsonObject->at("tiles").get();
	if (tilesElement && tilesElement->getType() == json::JsonElement::Type::JSON_ARRAY) {
	json::JsonArray* tilesArray = tilesElement->asArray();
	if (tilesArray) {
	std::vector<TilesetTile> tempTiles;
	for (const auto& tileElement : *tilesArray) {
	if (tileElement && tileElement->getType() == json::JsonElement::Type::JSON_OBJECT) {
	json::JsonObject* tileObject = tileElement->asObject();
	if (tileObject) {
	TilesetTile tile;
	tile.id = tileObject->at("id")->asInt();

	// 解析animation数组
	json::JsonElement* animationElement = tileObject->at("animation").get();
	if (animationElement && animationElement->getType() == json::JsonElement::Type::JSON_ARRAY) {
	json::JsonArray* animationArray = animationElement->asArray();
	if (animationArray) {
	for (const auto& frameElement : *animationArray) {
	if (frameElement && frameElement->getType() == json::JsonElement::Type::JSON_OBJECT) {
	json::JsonObject* frameObject = frameElement->asObject();
	if (frameObject) {
	TilesetAnimationFrame frame;
	frame.tileid = frameObject->at("tileid")->asInt();
	frame.duration = frameObject->at("duration")->asInt();
	tile.animation.push_back(frame);
	}
	}
	}
	}
	}

	tempTiles.push_back(tile);
	}
	}
	}
	const_cast<std::vector<TilesetTile>&>(tiles) = tempTiles;
	}
	}
}


bool TilesetLoader::loadFromFile(const std::string& jsonPath) {
	// 加载JSON文件
	json::JsonHandler jsonHandler(jsonPath);

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

	if (!parseData(jsonObject)) {
	return false;
	}

	std::filesystem::path jsonDir = std::filesystem::absolute(jsonPath).parent_path();
	std::filesystem::path imageFullPath = jsonDir / data.imagePath;
	if (!texture.loadFromFile(imageFullPath.string())) {
	std::cerr << "Failed to load texture: " << imageFullPath << std::endl;
	return false;
	}

	return true;
}

bool TilesetLoader::parseData(const json::JsonObject* jsonObject) {

	data = std::move(TilesetData(jsonObject));

	// 验证数据是否正确初始化
	if (data.columns == 0 && data.tileCount == 0) {
	std::cerr << "Failed to parse Tileset data" << std::endl;
	return false;
	}

	return true;
}