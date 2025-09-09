#include "Map.h"

#include <string>
#include <vector>

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
	// 初始化Layer属性
	// 	using JsonArray = std::vector<std::unique_ptr<JsonElement>>;
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
		// layer.opacity = static_cast<float>(layerObject->at("opacity")->asInt());
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
}

void Map::Draw(sf::RenderWindow& window)
{
	// TODO
}

void Map::Update(double deltaTime)
{
	// TODO
}