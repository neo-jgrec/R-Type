#ifndef CLIENTCOMPONENTS_HPP_
#define CLIENTCOMPONENTS_HPP_

#include <SFML/Graphics.hpp>

/**
 * @struct ViewComponent
 * @brief Component that represents a view in the game client.
 *
 * This component encapsulates an SFML View object, which defines
 * what region of the game world is visible on screen.
 */
struct ViewComponent {
    sf::View view; ///< The SFML View object that defines the visible area
};

struct EventComponent {
};

#endif /* !CLIENTCOMPONENTS_HPP_ */
