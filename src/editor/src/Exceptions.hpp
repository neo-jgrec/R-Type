/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Exceptions
*/

#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

#include <exception>
#include <string>

namespace Editor {
    class Exception : public std::exception {
        public:
            Exception(const std::string &message) : _message(message) {}
            const char *what() const noexcept override { return _message.c_str(); }

        protected:
        private:
            std::string _message;
    };

    class MainException : public Exception {
        public:
            MainException(const std::string &message) : Exception(message) {}
    };
}
#endif /* !EXCEPTIONS_HPP_ */