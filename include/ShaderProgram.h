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
        std::string readShaderFile(const char *path);
        void compileShader(GLuint shader, std::string shaderCode);
        GLuint linkProgram(GLuint vertShader, GLuint fragShader);
};

#endif
