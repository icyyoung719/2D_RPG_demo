#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>

#include "Animation.h"
#include "Player.h"

using namespace std;


int main() {
	sf::RenderWindow window(sf::VideoMode({512,512}), "SFML TEST");

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
	Animation idleAnimation(&idleTexture, sf::Vector2u(6, 1), 0.3f);
	Animation walkAnimation(&walkTexture, sf::Vector2u(8, 1), 0.3f);
	Animation jumpAnimation(&jumpTexture, sf::Vector2u(3, 1), 0.3f);
	Animation runAnimation(&runTexture, sf::Vector2u(6, 1), 0.3f);
	Animation flyingAnimation(&flyingTexture, sf::Vector2u(2, 1), 0.3f);

	std::map<Player::State, Animation> stateAnimationMap = {
		{Player::State::Idle, idleAnimation},
		{Player::State::Walk, walkAnimation},
		{Player::State::Jump, jumpAnimation},
		{Player::State::Run, runAnimation},
		{Player::State::Flying, flyingAnimation}
	};

	Player player(stateAnimationMap, 100.0f);	

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
	const auto onWindowResized = [&window](const sf::Event::Resized &resized)
	{
		std::cout << "New window size: " << resized.size.x << "x" << resized.size.y << std::endl;
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


		// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		// 	player.move({ -0.1f, 0.0f });
		// }
		// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		// 	player.move({ 0.1f, 0.0f });
		// }
		// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		// 	player.move({ 0.0f, -0.1f });
		// }
		// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		// 	player.move({ 0.0f, 0.1f });
		// }

		// if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
		// 	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		// 	player.setPosition(window.mapPixelToCoords(mousePos));
		// }

		player.Update(deltaTime);

		window.clear();
		player.Draw(window);
		window.display();
	}
	return 0;
}
