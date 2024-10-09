#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

sf::Vector2f getViewBounds(const sf::RenderWindow& window)
{
    sf::Vector2f center = window.getView().getCenter();
    sf::Vector2f size = window.getView().getSize();
    sf::FloatRect rect(
        (center.x - size.x / 2) - 16, (center.y - size.y / 2) - 16,
        size.x + 16, size.y + 16
    );

    return {rect.left, rect.top};
}
