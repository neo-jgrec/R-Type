#ifndef EVENT_HPP
    #define EVENT_HPP

#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <string>
#include <variant>
#include <ostream>
#include "../../../game/RequestType.hpp"
#include "../../../core/network/NetworkService.hpp"


/**
 * @struct NoData
 * @brief Represents an event that does not have a payload.
 */
struct NoData {
    /**
     * @brief Conversion operator to bool, used for checking the presence of data.
     * @return Always returns true as it signifies the presence of an empty payload.
     */
    operator bool() const {
        return true;
    }
};

/**
 * @class Event
 * @brief Represents an event that can occur in the system, which could have different types of payloads.
 *
 * The Event class encapsulates the type of event (such as PlayerMovement, ChatMessage, etc.)
 * and the corresponding payload, if applicable.
 */
class Event {
public:
    typedef std::variant<
        std::monostate,
        NoData,
        int,
        std::string,
        sf::Vector2u,
        std::pair<std::uint8_t, sf::Vector2u>,
        std::uint32_t,
        std::uint8_t> payload_t;

    /**
     * @brief Constructor for an event without a payload.
     * @param type The type of the event.
     */
    Event(RequestType type, const GDTPHeader& header, payload_t payload = std::monostate{});

    /**
     * @brief Returns the type of the event.
     * @return The EventType of the event.
     */
    [[nodiscard]] RequestType getType() const;

    /**
     * @brief Returns the payload of the event.
     *
     * This function returns the payload, which could be one of the various types depending on the event type.
     * If no data exists, it returns `std::monostate`.
     *
     * @return A variant containing the event payload.
     */
    [[nodiscard]] const payload_t& getPayload() const;

    /**
     * @brief Equality operator to compare two Event objects.
     * @param other The other Event object to compare with.
     * @return True if the event types and payloads are equal, otherwise false.
     */
    bool operator==(const Event& other) const {
        return this->type == other.type && this->payload == other.payload;
    }

    friend std::ostream& operator<<(std::ostream& os, const Event& event) {
        os << "Event{type=";
        switch (event.type) {
            case RequestType::GameStart:
                os << "GameStart";
                break;
            case RequestType::GameOver:
                os << "GameOver";
                break;
            case RequestType::PlayerConnect:
                os << "PlayerConnect";
                break;
            case RequestType::PlayerDisconnect:
                os << "PlayerDisconnect";
                break;
            case RequestType::PlayerMove:
                os << "PlayerMove";
                break;
            case RequestType::PlayerShoot:
                os << "PlayerShoot";
                break;
            case RequestType::PlayerCollide:
                os << "PlayerCollide";
                break;
            case RequestType::PlayerHit:
                os << "PlayerHit";
                break;
            case RequestType::PlayerDie:
                os << "PlayerDie";
                break;
            case RequestType::EnemySpawn:
                os << "EnemySpawn";
                break;
            case RequestType::EnemyMove:
                os << "EnemyMove";
                break;
            case RequestType::EnemyDie:
                os << "EnemyDie";
                break;
            case RequestType::MapScroll:
                os << "MapScroll";
                break;
            case RequestType::TileDestroy:
                os << "TileDestroy";
                break;
            default:
                os << "Unknown{" << event.type << "}";
                break;
        }

        std::cout << ", payload=";
        std::visit([&os](const auto& payload) {
            using T = std::decay_t<decltype(payload)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                os << "NoData";
            } else if constexpr (std::is_same_v<T, NoData>) {
                os << "NoData{}";
            } else if constexpr (std::is_same_v<T, int>) {
                os << "int{" << payload << "}";
            } else if constexpr (std::is_same_v<T, std::string>) {
                os << "string{" << payload << "}";
            } else if constexpr (std::is_same_v<T, sf::Vector2u>) {
                os << "Vector2u{x=" << payload.x << ", y=" << payload.y << "}";
            } else if constexpr (std::is_same_v<T, std::pair<std::uint8_t, sf::Vector2u>>) {
                os << "pair{first=" << static_cast<int>(payload.first) << ", second={x=" << payload.second.x << ", y=" << payload.second.y << "}}";
            } else if constexpr (std::is_same_v<T, std::uint32_t>) {
                os << "uint32_t{" << payload << "}";
            } else if constexpr (std::is_same_v<T, std::uint8_t>) {
                os << "uint8_t{" << static_cast<int>(payload) << "}";
            } else {
                static_assert(always_false<T>::value, "non-exhaustive visitor!");
            }
        }, event.payload);
        os << "}";
        return os;
    }

    [[nodiscard]] const GDTPHeader& getHeader() const
    {
        return header;
    }

private:
    template<typename>
    struct always_false : std::false_type {};
    RequestType type; ///< The type of the event.
    GDTPHeader header; /// Store the GDTPHeader
    payload_t payload; ///< The payload of the event.
};

#endif // EVENT_HPP
