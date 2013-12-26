#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>

class ShaderProgram {
    public:
        ShaderProgram(const char *vertShaderPath, const char *fragShaderPath);
        GLuint getProgram();
        void begin();
        void end();
        void setUniform(const char *location, glm::mat4 mat);
    private:
        GLuint program;
        bool active;
};

#endif
