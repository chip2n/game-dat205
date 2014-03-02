#ifndef SHADEREXCEPTION_H
#define SHADEREXCEPTION_H
#include <stdexcept>
#include <string>

class WindowException : public std::runtime_error {
    public:
        WindowException(const std::string& message) : std::runtime_error(message) {};
};

#endif
