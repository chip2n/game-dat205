#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "ShaderException.h"

ShaderProgram::ShaderProgram(const char *vertShaderPath, const char *fragShaderPath) {
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertShaderCode = readShaderFile(vertShaderPath);
    std::string fragShaderCode = readShaderFile(fragShaderPath);

    compileShader(vertShader, vertShaderCode);
    compileShader(fragShader, fragShaderCode);
    
    program = linkProgram(vertShader, fragShader);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

std::string ShaderProgram::readShaderFile(const char *path) {
    std::string shaderCode;
    std::ifstream shaderStream(path, std::ios::in);
    if(shaderStream.is_open()) {
        std::string line = "";
        while(getline(shaderStream, line)) {
            shaderCode += "\n" + line;
        }
        shaderStream.close();
    } else {
        throw ShaderException(std::string("Could not open file ") + path);
    }

    return shaderCode;
}

void ShaderProgram::compileShader(GLuint shader, std::string shaderCode) {
    GLint compileResult = GL_FALSE;
    int infoLogLength;

    char const *sourcePointer = shaderCode.c_str();
    glShaderSource(shader, 1, &sourcePointer, NULL);
    glCompileShader(shader);


    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if(compileResult == GL_FALSE) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> shaderErrorMessage(infoLogLength);
        glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderErrorMessage[0]);
        throw ShaderException(std::string("Error compiling shader: ") + &shaderErrorMessage[0]);
    }
}

GLuint ShaderProgram::linkProgram(GLuint vertShader, GLuint fragShader) {
    GLint compileResult = GL_FALSE;
    int infoLogLength;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &compileResult);
    if(compileResult == GL_FALSE) {
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> programErrorMessage(std::max(infoLogLength, int(1)));
        glGetProgramInfoLog(program, infoLogLength, NULL, &programErrorMessage[0]);
        throw ShaderException(std::string("Error linking shader program: ") + &programErrorMessage[0]);
    }

    return program;
}


GLuint ShaderProgram::getProgram() {
    return program;
}

void ShaderProgram::begin() {
    active = true;
    glUseProgram(program);
}

void ShaderProgram::end() {
    active = false;
    glUseProgram(0);
}

void ShaderProgram::setUniform(const char* location, glm::mat4 mat) {
    if(!active) {
        throw ShaderException("You need to call ShaderProgram.begin() before assigning uniforms.");
    }
    GLuint m = glGetUniformLocation(program, location);
    glUniformMatrix4fv(m, 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setUniform(const char* location, glm::vec3 vec) {
    if(!active) {
        throw ShaderException("You need to call ShaderProgram.begin() before assigning uniforms.");
    }
    GLuint m = glGetUniformLocation(program, location);
    glUniform3fv(m, 1, &vec[0]);
}

void ShaderProgram::setUniform(const char* location, int value) {
    if(!active) {
        throw ShaderException("You need to call ShaderProgram.begin() before assigning uniforms.");
    }
    GLuint m = glGetUniformLocation(program, location);
    glUniform1i(m, value);
}

void ShaderProgram::setUniform(const char* location, float value) {
    if(!active) {
        throw ShaderException("You need to call ShaderProgram.begin() before assigning uniforms.");
    }
    GLuint m = glGetUniformLocation(program, location);
    glUniform1f(m, value);
}
