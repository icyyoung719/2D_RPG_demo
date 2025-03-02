#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>

using namespace std;


const static float VIEW_HEIGHT = 512.0f;

int main() {
	sf::RenderWindow window(sf::VideoMode({512,512}), "Map Edited");
	sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_HEIGHT, VIEW_HEIGHT));

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

		window.handleEvents(onClose, onKeyPressed, onTextEntered);

		window.clear();
		window.setView(view);
		window.display();
	}
	return 0;
}
