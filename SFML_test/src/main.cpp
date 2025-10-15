#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>

#include "Animation.h"
#include "Player.h"
#include "Platform.h"
#include "TextureInfo.h"
#include "Tileset.h"
#include "Map.h"

using namespace std;


const static float VIEW_HEIGHT = 512.0f;

void ResizeView(const sf::RenderWindow &window, sf::View &view) {
	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(sf::Vector2f{VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT});
}

int main() {
	sf::RenderWindow window(sf::VideoMode({512,512}), "SFML TEST");
	sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_HEIGHT, VIEW_HEIGHT));

	std::string assestsBasePath = "../../../SFML_test/assests/";
	sf::Texture idleTexture,walkTexture,jumpTexture,runTexture,flyingTexture;
	bool ideLoaded = idleTexture.loadFromFile(assestsBasePath + "Hiker/Separated Animation PNGs/Idle.png");
	bool walkLoaded = walkTexture.loadFromFile(assestsBasePath + "Hiker/Separated Animation PNGs/Walk.png");
	bool jumpLoaded = jumpTexture.loadFromFile(assestsBasePath + "Hiker/Separated Animation PNGs/Jump.png");
	bool runLoaded = runTexture.loadFromFile(assestsBasePath + "Hiker/Separated Animation PNGs/Run.png");
	bool flyingLoaded = flyingTexture.loadFromFile(assestsBasePath + "Hiker/Separated Animation PNGs/Flying.png");
	if (!ideLoaded || !walkLoaded || !jumpLoaded || !runLoaded || !flyingLoaded) {
		std::cerr << "Error loading texture" << std::endl;
		return -1;
	}
	Animation idleAnimation(TextureInfo{ &idleTexture, sf::Vector2u(6, 1), 0.3f, sf::Vector2i(8, 8), sf::Vector2i(16, 24) });
	Animation walkAnimation(TextureInfo{ &walkTexture, sf::Vector2u(8, 1), 0.3f, sf::Vector2i(8, 8), sf::Vector2i(16, 24) });
	Animation jumpAnimation(TextureInfo{ &jumpTexture, sf::Vector2u(3, 1), 0.3f, sf::Vector2i(8, 8), sf::Vector2i(16, 24) });
	Animation runAnimation(TextureInfo{ &runTexture, sf::Vector2u(6, 1), 0.3f, sf::Vector2i(8, 8), sf::Vector2i(16, 24) });
	Animation flyingAnimation(TextureInfo{ &flyingTexture, sf::Vector2u(2, 1), 0.3f, sf::Vector2i(8, 8), sf::Vector2i(16, 24) });

	std::unordered_map<Player::State, Animation> stateAnimationMap = {
		{Player::State::Idle, idleAnimation},
		{Player::State::Walk, walkAnimation},
		{Player::State::Jump, jumpAnimation},
		{Player::State::Run, runAnimation},
		{Player::State::Flying, flyingAnimation}
	};

	Player player(stateAnimationMap, 32.0f);
	Tileset tileset;
	tileset.loadFromFile(assestsBasePath + "/world/overworld_tileset.json");
	Map map;
	std::string mapLocation{assestsBasePath + "/Maps/tiledMap.json"};
	map.Initialize(mapLocation);
	
	Platform platform1(nullptr, sf::Vector2f(400.0f, 200.0f), sf::Vector2f(500.0f, 200.0f));
	Platform platform2(nullptr, sf::Vector2f(400.0f, 200.0f), sf::Vector2f(500.0f, 0.0f));

	float deltaTime = 0.0f;
	sf::Clock clock;

	// float zoomFactor = 4.0f; // resize the view to 2 times bigger
	// view.zoom(1.0f / zoomFactor);

	const auto onClose = [&window](const sf::Event::Closed &)
	{
		window.close();
	};
	const auto onKeyPressed = [&window](const sf::Event::KeyPressed &keyPressed)
	{
		if (keyPressed.scancode == sf::Keyboard::Scancode::Escape)
			window.close();
		else if (keyPressed.scancode == sf::Keyboard::Scancode::C && keyPressed.control) {
			std::cout << "Ctrl+C pressed" << std::endl;
		}
	};
	const auto onWindowResized = [&window, &view](const sf::Event::Resized &resized)
	{
		std::cout << "New window size: " << resized.size.x << "x" << resized.size.y << std::endl;
		ResizeView(window, view);
	};
	const auto onTextEntered = [](const sf::Event::TextEntered &textEntered)
	{
		if (textEntered.unicode < 128)
		{
			std::cout << static_cast<char>(textEntered.unicode);
		}
	};
	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();


		window.handleEvents(onClose, onKeyPressed, onWindowResized, onTextEntered);

		player.Update(deltaTime);
		map.Update(deltaTime);

		auto playerCollider = player.GetCollider();
 		platform1.GetCollider().CheckCollision(playerCollider, 0.0f);
		platform2.GetCollider().CheckCollision(playerCollider, 1.0f);

		view.setCenter(player.GetPosition());

		window.clear();
		window.setView(view);
		map.Draw(window);
		player.Draw(window);
		platform1.Draw(window);
		platform2.Draw(window);
		window.display();
	}
	return 0;
}
