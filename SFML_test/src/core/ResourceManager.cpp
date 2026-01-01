#include "ResourceManager.h"
#include <iostream>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::setBasePath(const std::string& path) {
    basePath = path;
}

const std::string& ResourceManager::getBasePath() const {
    return basePath;
}

bool ResourceManager::loadTexture(const std::string& name, const std::string& filename) {
    auto texture = std::make_unique<sf::Texture>();
    std::string fullPath = basePath + filename;
    
    if (!texture->loadFromFile(fullPath)) {
        std::cerr << "Error loading texture: " << fullPath << std::endl;
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
