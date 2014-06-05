#ifndef SHADEREXCEPTION_H
#define SHADEREXCEPTION_H
#include <stdexcept>
#include <string>

class ShaderException : public std::runtime_error {
    public:
        ShaderException(const std::string& message) : std::runtime_error(message) {};
};

#endif
