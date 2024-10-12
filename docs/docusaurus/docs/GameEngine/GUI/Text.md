The `TextComponent` is a crucial part of the GUI system, allowing for the display of text within the game. It works in conjunction with other components like `DrawableComponent` and `SceneComponent` to render text in specific scenes and positions.

## Structure

```cpp
struct TextComponent {
    sf::Text text;
    sf::Font font;
};
```

- **text**: An SFML Text object that defines the content, style, and position of the text.
- **font**: An SFML Font object that specifies the font used for rendering the text.

## Creating Text

You can create text using the `createText` function in the `EntityFactory`. Here's an example of how it might be implemented:

```cpp
core::ecs::Entity EntityFactory::createText(core::ecs::Registry& registry, const std::string& content, const sf::Vector2f& position, int fontSize, const sf::Color& color, int scene) {
    core::ecs::Entity textEntity = registry.spawn_entity();

    sf::Font font;
    if (!font.loadFromFile("assets/Fonts/Arial.ttf")) {
        std::cerr << "Failed to load font: " << "assets/Fonts/Arial.ttf" << std::endl;
        return textEntity;
    }

    sf::Text text;
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(fontSize);
    text.setFillColor(color);
    text.setPosition(position);

    registry.add_component(textEntity, core::ge::TextComponent{text, font});
    registry.add_component(textEntity, core::ge::SceneComponent{scene});

    return textEntity;
}
```

This function:
- Creates a new text entity.
- Loads the font from a file.
- Sets up the text with the specified content, position, font size, and color.
- Adds a `TextComponent` and a `SceneComponent` to the entity.

## Drawing Text

To draw text in your game loop, you need to run the appropriate systems. Here's how you can do this:

```cpp
void Game::run() {
    while (_gameEngine.window.isOpen()) {
        // Handle events...

        // Update game state...

        // Clear the window
        _gameEngine.window.clear();

        // Draw components
        _gameEngine.registry.run_system<core::ge::TextComponent, core::ge::SceneComponent>();

        // Display the drawn content
        _gameEngine.window.display();
    }
}
```

This code:
- Clears the window at the start of each frame.
- Runs the `TextComponent` system along with the `SceneComponent` system.
- Displays the drawn content on the window.

## Text System

:::info
The text system is automatically registered when the `GameEngine` is initialized.
This code is here to explain how the text system works.
:::

The text system is responsible for rendering all text entities in the current scene:

```cpp
void textSystem() {
    registry.add_system<core::ge::TextComponent, core::ge::SceneComponent>([&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::TextComponent &text, core::ge::SceneComponent &scene) {
        if (scene.sceneName == currentScene) {
            window.draw(text.text);
        }
    });
}
```

This system:
- Checks if the text's scene matches the current scene.
- If it matches, draws the text on the window.

## Related Components
- [Button Component](Button.md)
- [Scene Component](Scenes.md)
- [Drawable Component](Drawable.md)

Each of these components can work together with the `TextComponent` to create interactive and visually appealing user interfaces in your game.
