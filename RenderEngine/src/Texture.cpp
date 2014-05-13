#include <iostream>
#include "SOIL.h"
#include "Texture.h"

Texture::Texture(std::string path) {
    loadFromFile(path);
}

void Texture::loadFromFile(std::string path) {
    texId = SOIL_load_OGL_texture(path.c_str(),
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS |
            SOIL_FLAG_INVERT_Y |
            SOIL_FLAG_NTSC_SAFE_RGB |
            SOIL_FLAG_COMPRESS_TO_DXT
    );
    /*
    int width, height, channels;
    unsigned char *data = SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    */

    if(0 == texId) {
        std::cout << "Error loading texture." << std::endl;
    }
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
