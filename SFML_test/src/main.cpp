#include <iostream>
#include "core/Game.h"
#include "core/ResourceManager.h"

int main() {
	// Set resource base path
	ResourceManager::getInstance().setBasePath("../../../SFML_test/assests/");

	// Create and run the game
	Game game;
	game.run();

	return 0;
}
