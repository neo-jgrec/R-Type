### Scene Component Documentation

The `SceneComponent` is essential for managing the visual and functional separation of entities across different scenes in the game. Each scene can contain various entities like buttons, text, and other drawable elements. By assigning a `SceneComponent` to an entity, the system ensures that it is rendered and updated only when the corresponding scene is active.

#### Structure

```cpp
struct SceneComponent {
    int sceneName;
};
```

- **sceneName**: This integer represents the identifier of the scene to which the entity belongs. Entities are only drawn or updated when the active scene matches their `sceneName`.

#### Usage Example
The `SceneComponent` is typically used when creating UI elements such as buttons or text. It ensures that these elements only appear in the appropriate scene.

For example, when you create a button and assign it to a scene:
```cpp
registry.add_component(button, core::ge::SceneComponent{scene});
```
This assigns the button to the specified scene.

#### Drawing Components

To properly draw components in your game loop, you need to run the appropriate systems for each component type. Refer to the documentation of each component to see which systems you need to run.

#### Related Components
To learn more about how to create and manage these entities, check the following documentation:

- [Button Component](Button.md)
- [Drawable Component](Drawable.md)
- [Text Component](Text.md)

Each of these components can be assigned to entities along with the `SceneComponent` to ensure they are properly rendered and interactable within their assigned scenes.
