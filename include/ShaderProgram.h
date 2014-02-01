#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h> 
#include <string>
class ShaderProgram {
    public:
        ShaderProgram(const char *vertShaderPath, const char *fragShaderPath);
        GLuint getProgram();
        void begin();
        void end();
        void setUniform(const char *location, glm::mat4 mat);
        void setUniform(const char *location, glm::vec3 vec);
    private:
        GLuint program;
        bool active;
        std::string readShaderFile(const char *path);
        void compileShader(GLuint shader, std::string shaderCode);
        GLuint linkProgram(GLuint vertShader, GLuint fragShader);
};

#endif
