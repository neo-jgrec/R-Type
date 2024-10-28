#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../../../core/ecs/GameEngine/GameEngine.hpp"
#include "../../../game/Components.hpp"
#include "src/Game/Utils/ClientComponents.hpp"
#include "Game.hpp"

std::shared_ptr<sf::Texture> loadTextureOrRed(const std::string& filePath)
{
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(filePath)) {
        std::cerr << "Failed to load texture: " << filePath << ". Using red texture." << std::endl;
        texture->create(24, 24);
        sf::Image red;
        red.create(24, 24, sf::Color::Red);
        texture->update(red);
    } else {
        std::cout << "Loaded texture: " << filePath << std::endl;
    }
    return texture;
}

void Game::initBackground(core::ecs::Registry& registry, nlohmann::json& mapData, sf::RenderWindow& window, sf::Vector2f& gameScale)
{
    if (!mapData.contains("background")) {
        std::cerr << "Error: Missing essential map data fields." << std::endl;
        return;
    }

    auto backgroundData = mapData["background"];

    if (!backgroundData.contains("path") || !backgroundData.contains("height")) {
        std::cerr << "Error: Missing essential map data fields." << std::endl;
        return;
    }

    std::string backgroundPath = backgroundData["path"];
    float imageHeight = backgroundData["height"].get<float>() * mapData["cellSize"].get<float>();
    float scale = static_cast<float>(window.getSize().y) / imageHeight;

    auto backgroundTexture = loadTextureOrRed(backgroundPath);
    if (!backgroundTexture) {
        std::cerr << "Error: Failed to load background texture." << std::endl;
        return;
    }

    float mapWidth = mapData["width"].get<float>() * mapData["cellSize"].get<float>() * gameScale.x;
    float backgroundWidth = static_cast<float>(backgroundTexture->getSize().x) * scale;
    int numBackgrounds = std::ceil(mapWidth / backgroundWidth);

    for (int i = 0; i < numBackgrounds; ++i) {
        core::ecs::Entity backgroundEntity = registry.spawn_entity();

        sf::RectangleShape backgroundShape(sf::Vector2f(static_cast<float>(backgroundTexture->getSize().x) * scale, static_cast<float>(window.getSize().y)));
        backgroundShape.setTexture(backgroundTexture.get());
        backgroundShape.setPosition(static_cast<float>(i) * backgroundWidth, 0);
        backgroundShape.setScale(scale, scale);

        registry.add_component(backgroundEntity, core::ge::TransformComponent{
            sf::Vector2f(static_cast<float>(i) * backgroundWidth, 0),
            sf::Vector2f(static_cast<float>(backgroundTexture->getSize().x) * scale, static_cast<float>(window.getSize().y)),
            sf::Vector2f(1.0f, 1.0f),
            0.0f
        });
        registry.add_component(backgroundEntity, core::ge::DrawableComponent{backgroundShape});
        registry.add_component(backgroundEntity, core::ge::TextureComponent{backgroundTexture});
    }
}

void Game::parseMap(Game &game, const std::string& mapFilePath, sf::RenderWindow& window)
{
    auto& gameEngine = game.getGameEngine();
    auto& registry = gameEngine.registry;

    std::ifstream mapFile(mapFilePath);

    if (!mapFile) {
        std::cerr << "Error: Could not open map file: " << mapFilePath << std::endl;
        return;
    }

    nlohmann::json mapData;
    try {
        mapFile >> mapData;
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to parse JSON from map file: " << e.what() << std::endl;
        return;
    }

    if (!mapData.contains("tileSets") || !mapData.contains("tiles") ||
        !mapData.contains("height") || !mapData.contains("width") || !mapData.contains("cellSize")) {
        std::cerr << "Error: Missing essential map data fields." << std::endl;
        return;
    }

    std::unordered_map<int, sf::IntRect> tileRects;
    std::unordered_map<int, std::shared_ptr<sf::Texture>> tileTextures;
    int tileIndex = 0;

    try {
        for (const auto& tileSet : mapData["tileSets"]) {
            if (!tileSet.contains("filePath") || !tileSet.contains("tileCount") ||
                !tileSet.contains("tileWidth") || !tileSet.contains("tileHeight")) {
                std::cerr << "Warning: Tile set missing essential fields." << std::endl;
                continue;
            }

            std::string filePath = tileSet["filePath"];
            int tileWidth = tileSet["tileWidth"];
            int tileHeight = tileSet["tileHeight"];
            int tileCount = tileSet["tileCount"];

            auto texture = loadTextureOrRed(filePath);
            int tilesPerRow = static_cast<int>(texture->getSize().x) / tileWidth;

            for (int i = 0; i < tileCount; i++) {
                int x = (i % tilesPerRow) * tileWidth;
                int y = (i / tilesPerRow) * tileHeight;

                tileRects[tileIndex] = sf::IntRect(x, y, tileWidth, tileHeight);
                tileTextures[tileIndex] = texture;
                tileIndex++;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Exception while loading tile textures: " << e.what() << std::endl;
        return;
    }

    float mapHeight = mapData["height"].get<float>() * mapData["cellSize"].get<float>();
    float scale = static_cast<float>(window.getSize().y) / mapHeight;

    gameScale = sf::Vector2f(scale, scale);

    initBackground(gameEngine.registry, mapData, window, gameScale);

    for (const auto& tile : mapData["tiles"]) {
        if (!tile.contains("tileIndex") || !tile.contains("x") || !tile.contains("y") || !tile.contains("isDestructible")) {
            std::cerr << "Warning: Tile data missing essential fields. Skipping tile." << std::endl;
            continue;
        }

        try {
            core::ecs::Entity tileEntity = gameEngine.registry.spawn_entity();
            int tileIdx = tile["tileIndex"].get<int>() - 1;
            if (tileIdx < 0)
                continue;

            bool isDestructible = tile["isDestructible"].get<bool>();
            const sf::Vector2f size{mapData["cellSize"].get<float>() * gameScale.x, mapData["cellSize"].get<float>() * gameScale.y};

            sf::RectangleShape tileShape(size);
            tileShape.setTexture(tileTextures[tileIdx].get());
            tileShape.setTextureRect(tileRects[tileIdx]);

            registry.add_component(tileEntity, core::ge::TransformComponent{{tile["x"].get<float>() * size.x, tile["y"].get<float>() * size.y}, size, {1.0f, 1.0f}, 0.0f});
            registry.add_component(tileEntity, core::ge::DrawableComponent{tileShape});
            registry.add_component(tileEntity, core::ge::TextureComponent{tileTextures[tileIdx]});
            registry.add_component(tileEntity, TileComponent{isDestructible, {
                static_cast<uint32_t>(tile["x"].get<float>()) * static_cast<uint32_t>(mapData["cellSize"].get<float>()),
                static_cast<uint32_t>(tile["y"].get<float>()) * static_cast<uint32_t>(mapData["cellSize"].get<float>())
            }});
        } catch (const std::exception& e) {
            std::cerr << "Error: Exception while parsing tile data: " << e.what() << std::endl;
        }
    }

    std::cout << "Map parsed successfully." << std::endl;
}
