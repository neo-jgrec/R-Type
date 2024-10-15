#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "Game.hpp"
#include "../../../game/Components.hpp"
#include "../../../game/CollisionMask.hpp"
#include "src/Game/Utils/ClientComponents.hpp"

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

void Game::parseMap(core::ecs::Registry& registry, const std::string& mapFilePath, sf::RenderWindow& window)
{
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

    try {
        _tileMap.resize(mapData["height"]);
        for (auto& row : _tileMap) {
            row.resize(mapData["width"]);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Exception while resizing tile map: " << e.what() << std::endl;
        return;
    }

    float mapHeight = mapData["height"].get<float>() * mapData["cellSize"].get<float>();
    float scale = static_cast<float>(window.getSize().y) / mapHeight;

    gameScale = sf::Vector2f(scale, scale);

    for (const auto& tile : mapData["tiles"]) {
        if (!tile.contains("tileIndex") || !tile.contains("x") || !tile.contains("y") || !tile.contains("isDestructible")) {
            std::cerr << "Warning: Tile data missing essential fields. Skipping tile." << std::endl;
            continue;
        }

        try {
            core::ecs::Entity tileEntity = registry.spawn_entity();
            int tileIdx = tile["tileIndex"].get<int>() - 1;
            bool isDestructible = tile["isDestructible"].get<bool>();
            sf::Vector2f tilePos(tile["x"].get<float>() * mapData["cellSize"].get<float>() * gameScale.x,
                                 tile["y"].get<float>() * mapData["cellSize"].get<float>() * gameScale.y);

            sf::RectangleShape tileShape(sf::Vector2f(mapData["cellSize"].get<float>() * gameScale.x, mapData["cellSize"].get<float>() * gameScale.y));
            tileShape.setTexture(tileTextures[tileIdx].get());
            tileShape.setTextureRect(tileRects[tileIdx]);

            registry.add_component(tileEntity, core::ge::TransformComponent{tilePos, {mapData["cellSize"].get<float>() * gameScale.x, mapData["cellSize"].get<float>() * gameScale.y}, {1.0f, 1.0f}, 0.0f});
            registry.add_component(tileEntity, core::ge::DrawableComponent{tileShape});
            registry.add_component(tileEntity, core::ge::TextureComponent{tileTextures[tileIdx]});
            registry.add_component(tileEntity, core::ge::SceneComponent{static_cast<int>(GameState::Playing)});
            registry.add_component(tileEntity, TileComponent{isDestructible});

            registry.add_component(tileEntity, core::ge::CollisionComponent{
                WORLD, {sf::FloatRect(0.0f, 0.0f, mapData["cellSize"].get<float>() * gameScale.x, mapData["cellSize"].get<float>() * gameScale.y)},
                {
                    {PLAYER_PROJECTILE, [&](const core::ecs::Entity self, const core::ecs::Entity other) {
                        const auto& tileOpt = registry.get_components<TileComponent>()[self];
                        if (tileOpt.has_value()) {
                            const auto& tile = tileOpt.value();
                            if (tile->isDestructible)
                                registry.kill_entity(self);
                        } else {
                            std::cerr << "Error: TileComponent not found for entity." << std::endl;
                        }
                        registry.kill_entity(other);
                    }},
                    {PLAYER_MISSILE, [&](const core::ecs::Entity self, [[maybe_unused]] const core::ecs::Entity other) {
                        const auto& tileOpt = registry.get_components<TileComponent>()[self];
                        if (tileOpt.has_value()) {
                            const auto& tile = tileOpt.value();
                            if (tile->isDestructible)
                                registry.kill_entity(self);
                        } else {
                            std::cerr << "Error: TileComponent not found for entity." << std::endl;
                        }

                        const auto& healthOpt = registry.get_components<HealthComponent>()[other];
                        if (healthOpt.has_value()) {
                            auto& health = healthOpt.value();
                            health->health -= 10;
                            if (health->health <= 0)
                                registry.kill_entity(other);
                        } else {
                            std::cerr << "Error: HealthComponent not found for entity." << std::endl;
                        }
                    }},
                    {PLAYER, [&](const core::ecs::Entity, const core::ecs::Entity other) {
                        if (isDestructible) {
                            registry.kill_entity(other);
                        }
                    }},
                }
            });

            _tileMap[tile["y"]][tile["x"]] = Tile{tileEntity, tilePos, isDestructible};
        } catch (const std::exception& e) {
            std::cerr << "Error: Exception while parsing tile data: " << e.what() << std::endl;
            continue;
        }
    }

    std::cout << "Map parsed successfully." << std::endl;
}
