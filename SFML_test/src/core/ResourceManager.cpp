#include "ResourceManager.h"
#include <iostream>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::setBasePath(const std::filesystem::path& path) {
    basePath = path;
}

const std::filesystem::path& ResourceManager::getBasePath() const {
    return basePath;
}

bool ResourceManager::loadTexture(const std::string& name, const std::filesystem::path& filename) {
    auto texture = std::make_unique<sf::Texture>();
    std::filesystem::path fullPath = basePath / filename;
    
    if (!texture->loadFromFile(fullPath.string())) {
        std::cerr << "Error loading texture: " << fullPath << std::endl;
        std::cerr << "File absolute path: " << std::filesystem::absolute(fullPath) << std::endl;
        return false;
    }
    
    textures[name] = std::move(texture);
    return true;
}

sf::Texture* ResourceManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second.get();
    }
    return nullptr;
}
