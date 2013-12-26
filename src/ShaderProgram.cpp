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

    std::string vertShaderCode;
    std::ifstream vertShaderStream(vertShaderPath, std::ios::in);
    if(vertShaderStream.is_open()) {
        std::string line = "";
        while(getline(vertShaderStream, line)) {
            vertShaderCode += "\n" + line;
        }
        vertShaderStream.close();
    } else {
        throw ShaderException(std::string("Could not open file ") + vertShaderPath);
    }

    std::string fragShaderCode;
    std::ifstream fragShaderStream(fragShaderPath, std::ios::in);
    if(fragShaderStream.is_open()) {
        std::string line = "";
        while(getline(fragShaderStream, line)) {
            fragShaderCode += "\n" + line;
        }
        fragShaderStream.close();
    } else {
        throw ShaderException(std::string("Could not open file ") + fragShaderPath);
    }
    
    GLint compileResult = GL_FALSE;
    int infoLogLength;

    printf("Compiling shader : %s\n", vertShaderPath);
    char const *vertSourcePointer = vertShaderCode.c_str();
    glShaderSource(vertShader, 1, &vertSourcePointer, NULL);
    glCompileShader(vertShader);


    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compileResult);
    if(compileResult == GL_FALSE) {
        glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> vertShaderErrorMessage(infoLogLength);
        glGetShaderInfoLog(vertShader, infoLogLength, NULL, &vertShaderErrorMessage[0]);
        throw ShaderException(std::string("Error compiling vertex shader: ") + &vertShaderErrorMessage[0]);
    }



    printf("Compiling shader : %s\n", fragShaderPath);
    char const *fragSourcePointer = fragShaderCode.c_str();
    glShaderSource(fragShader, 1, &fragSourcePointer, NULL);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileResult);
    if(compileResult == GL_FALSE) {
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> fragShaderErrorMessage(infoLogLength);
        glGetShaderInfoLog(fragShader, infoLogLength, NULL, &fragShaderErrorMessage[0]);
        throw ShaderException(std::string("Error compiling fragment shader: ") + &fragShaderErrorMessage[0]);
    }

    printf("Linking program\n");
    program = glCreateProgram();
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

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

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
