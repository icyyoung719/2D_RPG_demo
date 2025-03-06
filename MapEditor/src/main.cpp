#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>

#include "Grid.h"
#include "MouseTile.h"

using namespace std;


const static float VIEW_HEIGHT = 512.0f;

int main() {
	sf::RenderWindow window(sf::VideoMode({512*2,512}), "Map Editor");
	sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_HEIGHT*2, VIEW_HEIGHT));
	view.setCenter({static_cast<float>(window.getSize().x) / 2, static_cast<float>(window.getSize().y) / 2});


	std::string assestsBasePath = "../../../../SFML_test/assests/";
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

	Grid grid({100.0, 100.0}, {16, 16}, {10, 5}, {4, 4}, 2, sf::Color{255, 0, 0, 255});
	grid.Initialize();
	grid.Load();
	MouseTile mouse_tile{{4.0f,4.0f}, {16,16}, {100.0f, 100.0f}};
	mouse_tile.Initialize();
	mouse_tile.Load();

	float deltaTime = 0.0f;
	sf::Clock clock;


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
		sf::Vector2f mousePosition = sf::Vector2f{sf::Mouse::getPosition(window)};

		window.handleEvents(onClose, onKeyPressed, onTextEntered);
		grid.Update(deltaTime);
		mouse_tile.Update(deltaTime, mousePosition);

		window.clear();
		window.setView(view);
		grid.Draw(window);
		mouse_tile.Draw(window);
		window.display();
	}
	return 0;
}
