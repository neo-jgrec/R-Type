/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** InvalidEventConversion
*/

#pragma once

#include <exception>
#include <cstdint>
#include <string>
/**
 * @class InvalidEventConversion
 * @brief Exception class for handling invalid conversions between uint8_t and EventType.
 *
 * This exception is thrown when an invalid or unsupported value is passed during the conversion
 * from a `uint8_t` to an `EventType`.
 */
class InvalidEventConversion : std::exception {
public:
    /**
     * @brief Constructor for InvalidEventConversion.
     *
     * Initializes the exception with the invalid `uint8_t` value that caused the conversion failure.
     *
     * @param value The `uint8_t` value that caused the invalid conversion.
     */
    InvalidEventConversion(uint8_t value) : _value(value) {}

    /**
     * @brief Deleted default constructor.
     *
     * This constructor is deleted to ensure that the exception must always be initialized
     * with a specific invalid value.
     */
    InvalidEventConversion() = delete;

    /**
     * @brief Provides a descriptive error message for the invalid conversion.
     *
     * Overrides the `std::exception::what()` method to return a dynamically generated error message
     * indicating the invalid value that caused the conversion error.
     *
     * @return A C-string containing the error message.
     */
    const char *what() const noexcept override {
        return std::string("Invalid value for EventType conversion: " + std::to_string(this->_value)).c_str();
    }

private:
    uint8_t _value;  ///< The invalid `uint8_t` value that caused the conversion failure.
};
