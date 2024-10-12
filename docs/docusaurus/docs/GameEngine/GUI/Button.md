The `ButtonComponent` represents a clickable UI element within the ECS architecture. It can be visually customized and linked to specific functionality through an `onClick` event. This component is used in conjunction with other components like `DrawableComponent`, `TextComponent`, and `SceneComponent` to provide a fully interactive button in the game.

## Structure

```cpp
struct ButtonComponent {
    sf::RectangleShape shape;
    std::function<void()> onClick;
    bool isHovered = false;
    bool isPressed = false;
};
```

- **shape**: Defines the visual appearance of the button, such as its size, position, and color.
- **onClick**: The function that is called when the button is clicked.
- **isHovered**: A boolean that determines if the mouse is currently hovering over the button.
- **isPressed**: A boolean that indicates if the button is being pressed.

## Creating a Button

You can create a button using the `createButton` function, which initializes a new entity and assigns the necessary components:

```cpp
core::ecs::Entity EntityFactory::createButton(core::ecs::Registry& registry, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& label, const std::function<void()>& onClick, int scene) {
    core::ecs::Entity button = registry.spawn_entity();

    sf::RectangleShape shape(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);

    sf::Font font;
    if (!font.loadFromFile("assets/Fonts/Arial.ttf")) {
        std::cerr << "Failed to load font: " << "assets/Fonts/Arial.ttf" << std::endl;
        return button;
    }

    sf::Text text;
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);

    registry.add_component(button, core::ge::DrawableComponent{shape});
    registry.add_component(button, core::ge::TextComponent{text, font});
    registry.add_component(button, core::ge::ButtonComponent{shape, onClick, false, false});
    registry.add_component(button, core::ge::SceneComponent{scene});
    return button;
}
```

This function:
- Creates a new button entity.
- Adds a `DrawableComponent`, `TextComponent`, and `ButtonComponent` to the entity.
- Links the button to a specific scene using the `SceneComponent`.

## Drawing Buttons

To draw buttons in your game loop, you need to run the appropriate systems. Here's how you can do this:

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
- Runs the `TextComponent` system.
- Displays the drawn content on the window.

By running these systems, you ensure that all drawable components (including buttons) and text components are rendered each frame. The `SceneComponent` is used in conjunction with these systems to only draw elements that belong to the current scene.

:::note
Remember that the button system (which handles interactions) should be run before these drawing systems in your game loop to ensure the button states are updated before rendering. See below for more information.
:::

## Updating Buttons

To update buttons in your game loop, you need to run the appropriate systems. Here's how you can do this:

```cpp
void Game::update() {
    _gameEngine.registry.run_system<core::ge::ButtonComponent, core::ge::SceneComponent, core::ge::DrawableComponent, core::ge::TextComponent>();
}
```

## Button System

:::info

The button system is automatically registered when the `GameEngine` is initialized.
This code is here to explain how the button system works.

:::

The button system manages the interaction with buttons by handling mouse events, updating the `isHovered` and `isPressed` states, and invoking the `onClick` function when the button is pressed.

```cpp
void buttonSystem() {
    registry.add_system<core::ge::ButtonComponent, core::ge::SceneComponent, core::ge::DrawableComponent, core::ge::TextComponent>([&window = window, &currentScene = currentScene](core::ecs::Entity, core::ge::ButtonComponent &button, core::ge::SceneComponent &scene, core::ge::DrawableComponent &drawable, core::ge::TextComponent &text) {
        if (scene.sceneName != currentScene)
            return;
        
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePosition);

        button.isHovered = button.shape.getGlobalBounds().contains(worldPos);
        button.isPressed = button.isHovered && sf::Mouse::isButtonPressed(sf::Mouse::Left);

        if (button.isPressed) {
            button.onClick();
            drawable.shape.setSize(button.shape.getSize() * 0.98f);
            text.text.setScale(sf::Vector2f(0.98f, 0.98f));
        } else if (button.isHovered) {
            drawable.shape.setSize(button.shape.getSize() * 1.02f);
            text.text.setScale(sf::Vector2f(1.02f, 1.02f));
        } else {
            drawable.shape.setSize(button.shape.getSize());
            text.text.setScale(sf::Vector2f(1.0f, 1.0f));
        }
    });
}
```

This system:
- Checks if the mouse is hovering over the button.
- Adjusts the size of the button and text when hovered or pressed.
- Calls the `onClick` function when the button is clicked.

#### Related Components
- [Drawable Component](link-to-drawable-doc)
- [Text Component](link-to-text-doc)
- [Scene Component](link-to-scene-doc)

Each of these components works together to provide the full functionality of the button.