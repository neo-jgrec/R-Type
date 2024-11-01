#ifndef UI_COMPONENTS_HPP_
#define UI_COMPONENTS_HPP_

#include "GameEngine.hpp"
#include "GameEngineComponents.hpp"
#include <functional>

namespace core::ge {

class UIFactory {
public:
    static ecs::Entity createButton(GameEngine& engine, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& label, const std::function<void()>& onClick) {
        auto& registry = engine.registry;
        ecs::Entity button = registry.spawn_entity();

        #ifdef GE_USE_SDL
            SDL_Rect rect = {static_cast<int>(position.x), static_cast<int>(position.y), 
                            static_cast<int>(size.x), static_cast<int>(size.y)};
            
            auto font = engine.assetManager.getFont("arial");
            SDL_Color textColor = {0, 0, 0, 255};
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, label.c_str(), textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(engine.renderer, textSurface);
            SDL_FreeSurface(textSurface);

            registry.add_component(button, TransformComponent{position, size, {1.0f, 1.0f}, 0.0f});
            registry.add_component(button, DrawableComponent{rect, nullptr});
            registry.add_component(button, TextComponent{
                .text = rect,
                .textTexture = textTexture,
                .font = font
            });
        #else
            sf::RectangleShape shape(size);
            shape.setPosition(position);
            shape.setFillColor(sf::Color::White);
            shape.setOutlineThickness(2);
            shape.setOutlineColor(sf::Color::Black);

            auto font = engine.assetManager.getFont("arial");
            sf::Text text;
            text.setFont(font);
            text.setString(label);
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::Black);

            sf::FloatRect textBounds = text.getLocalBounds();
            text.setOrigin(textBounds.left + textBounds.width / 2.0f, 
                          textBounds.top + textBounds.height / 2.0f);
            text.setPosition(position.x + size.x / 2.0f, 
                           position.y + size.y / 2.0f);

            registry.add_component(button, TransformComponent{position, size, {1.0f, 1.0f}, 0.0f});
            registry.add_component(button, DrawableComponent{shape});
            registry.add_component(button, TextComponent{text, font});
        #endif

        registry.add_component(button, ClickableComponent{false, false, onClick});
        return button;
    }

    static ecs::Entity createSlider(GameEngine& engine, const sf::Vector2f& position, const sf::Vector2f& size, 
                                  const std::string& label, const std::function<void(float)>& onChange, float currentValue) {
        auto& registry = engine.registry;
        ecs::Entity slider = registry.spawn_entity();

        #ifdef GE_USE_SDL
            SDL_Rect barRect = {static_cast<int>(position.x), static_cast<int>(position.y),
                               static_cast<int>(size.x), static_cast<int>(size.y)};
            
            auto font = engine.assetManager.getFont("arial");
            SDL_Color textColor = {255, 255, 255, 255};
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, label.c_str(), textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(engine.renderer, textSurface);
            SDL_FreeSurface(textSurface);

            SDL_Rect textRect = {static_cast<int>(position.x), static_cast<int>(position.y - 40),
                                static_cast<int>(size.x), 30};

            registry.add_component(slider, SliderComponent{0, 100, currentValue, barRect, {}, onChange});
            registry.add_component(slider, TextComponent{textRect, textTexture, font});
        #else
            sf::RectangleShape bar(size);
            bar.setPosition(position);
            bar.setFillColor(sf::Color::White);
            bar.setOutlineThickness(2);
            bar.setOutlineColor(sf::Color::Black);

            sf::CircleShape handle(15);
            handle.setPosition(position.x + size.x, position.y + size.y / 2);
            handle.setFillColor(sf::Color::Red);

            auto font = engine.assetManager.getFont("arial");
            sf::Text text;
            text.setFont(font);
            text.setString(label);
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            text.setPosition(position.x, position.y - 40.0f);

            registry.add_component(slider, SliderComponent{0, 100, currentValue, bar, handle, onChange});
            registry.add_component(slider, TextComponent{text, font});
        #endif

        return slider;
    }

    static ecs::Entity createTextInput(GameEngine& engine, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& title) {
        auto& registry = engine.registry;
        ecs::Entity textInput = registry.spawn_entity();

        #ifdef GE_USE_SDL
            SDL_Rect rect = {static_cast<int>(position.x), static_cast<int>(position.y),
                            static_cast<int>(size.x), static_cast<int>(size.y)};
            
            auto font = engine.assetManager.getFont("arial");
            SDL_Color textColor = {255, 255, 255, 255};
            
            // Title text
            SDL_Surface* titleSurface = TTF_RenderText_Solid(font, title.c_str(), textColor);
            SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(engine.renderer, titleSurface);
            SDL_FreeSurface(titleSurface);
            
            SDL_Rect titleRect = {static_cast<int>(position.x), static_cast<int>(position.y - 40),
                                 static_cast<int>(size.x), 30};

            // Input text
            SDL_Color inputColor = {0, 0, 0, 255};
            SDL_Surface* inputSurface = TTF_RenderText_Solid(font, "", inputColor);
            SDL_Texture* inputTexture = SDL_CreateTextureFromSurface(engine.renderer, inputSurface);
            SDL_FreeSurface(inputSurface);
            
            SDL_Rect inputRect = {static_cast<int>(position.x + 10), static_cast<int>(position.y + 10),
                                 static_cast<int>(size.x - 20), static_cast<int>(size.y - 20)};

            registry.add_component(textInput, TextComponent{titleRect, titleTexture, font});
            registry.add_component(textInput, DrawableComponent{rect, nullptr});
            registry.add_component(textInput, TextInputComponent{inputRect, font, false, 0, 20});
        #else
            sf::RectangleShape shape(size);
            shape.setPosition(position);
            shape.setFillColor(sf::Color::White);
            shape.setOutlineThickness(2);
            shape.setOutlineColor(sf::Color::Black);

            auto font = engine.assetManager.getFont("arial");

            // Title text
            sf::Text titleText;
            titleText.setFont(font);
            titleText.setString(title);
            titleText.setCharacterSize(24);
            titleText.setFillColor(sf::Color::White);
            titleText.setPosition(position.x, position.y - 40.0f);

            // Input text
            sf::Text inputText;
            inputText.setFont(font);
            inputText.setString("");
            inputText.setCharacterSize(24);
            inputText.setFillColor(sf::Color::Black);
            inputText.setPosition(position.x + 10.0f, position.y + 10.0f);

            registry.add_component(textInput, TextComponent{titleText, font});
            registry.add_component(textInput, DrawableComponent{shape});
            registry.add_component(textInput, TextInputComponent{inputText, font, false, 0, 20});
        #endif

        return textInput;
    }

    static ecs::Entity createImage(GameEngine& engine, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& textureName) {
        auto& registry = engine.registry;
        ecs::Entity img = registry.spawn_entity();

        #ifdef GE_USE_SDL
            SDL_Rect rect = {static_cast<int>(position.x), static_cast<int>(position.y),
                            static_cast<int>(size.x), static_cast<int>(size.y)};
            
            auto texture = engine.assetManager.getTexture(textureName);
            
            registry.add_component(img, DrawableComponent{rect, texture});
            registry.add_component(img, TextureComponent{texture});
        #else
            sf::RectangleShape shape(size);
            shape.setPosition(position);

            auto texture = engine.assetManager.getTexture(textureName);
            shape.setTexture(texture.get());

            registry.add_component(img, DrawableComponent{shape});
            registry.add_component(img, TextureComponent{texture});
        #endif

        return img;
    }
};

} // namespace core::ge

#endif // UI_COMPONENTS_HPP_