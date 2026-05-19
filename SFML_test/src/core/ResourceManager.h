#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>

class ResourceManager {
public:
    static ResourceManager& getInstance();

    // Delete copy and move constructors
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    void setBasePath(const std::filesystem::path& path);
    const std::filesystem::path& getBasePath() const;

    // Load a texture and store it
    bool loadTexture(const std::string& name, const std::filesystem::path& filename);
    
    // Get a texture by name
    sf::Texture* getTexture(const std::string& name);

private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    std::filesystem::path basePath;
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
};
