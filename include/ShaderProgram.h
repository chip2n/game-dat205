#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>

class ShaderProgram {
    public:
        ShaderProgram(const char *vertShaderPath, const char *fragShaderPath);
        GLuint getProgram();
    private:
        GLuint program;
};

#endif
