#pragma once
#include <SFML/Graphics.hpp>
#include "TextureInfo.h"

class Animation
{
public:
	Animation(const TextureInfo& textureInfo);
	Animation();
	~Animation();

	void Update(int row, float deltaTime, bool faceRight);
	sf::Texture* getTexture() { return textureInfo.texture; }

private:
	

public:
	// determine the position and size of the texture(despites the real size of the texture)
	sf::IntRect standardUvRect;
	// real model size after removing the transparent part
	sf::IntRect textureRealUvRect;

private:
	TextureInfo textureInfo;
	sf::Vector2u currentImage;
	float totalTime;
};