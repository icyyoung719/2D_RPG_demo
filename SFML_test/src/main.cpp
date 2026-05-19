#include <iostream>
#include <filesystem>
#include "core/Game.h"
#include "core/ResourceManager.h"

int main(int argc, char* argv[]) {
	// Set resource base path relative to executable
	std::filesystem::path exePath = std::filesystem::absolute(std::filesystem::path(argv[0]));
	std::filesystem::path exeDir = exePath.parent_path();
	ResourceManager::getInstance().setBasePath(exeDir / "assests");

	// Create and run the game
	Game game;
	game.run();

	return 0;
}
