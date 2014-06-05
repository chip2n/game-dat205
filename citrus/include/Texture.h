#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

class Texture {
    public:
        Texture(std::string path);
        void bind();
        void unbind();
    private:
        void loadFromFile(std::string path);
        GLuint texId;
};

#endif
