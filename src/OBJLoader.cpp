#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <glm/glm.hpp>
#include "OBJLoader.h"
#include <stdio.h>

void OBJLoader::loadOBJ(std::string path, std::vector<glm::vec3> &outVertices, std::vector<glm::vec2> &outTexCoords) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;

    std::ifstream file(path.c_str());

    std::string line;
    while(std::getline(file, line)) {
        switch(line[0]) {
        case 'v':
            if(line[1] == ' ')
                vertices.push_back(readVertex(line));
            else if(line[1] == 't')
                texCoords.push_back(readTexCoord(line));
            break;
        case 'f':
            line.erase(0, line.find(' ') + 1);

            for(int i = 0; i < 3; i++) {
                std::string currentFace = line.substr(0, line.find(' '));
                int vertexIndex;
                int texCoordIndex;
                int normalIndex;
                sscanf(line.c_str(), "%d/%d/%d", &vertexIndex, &texCoordIndex, &normalIndex);
                std::cout << vertexIndex << "/" << texCoordIndex << "/" << normalIndex << std::endl;
                line.erase(0, line.find(' ') + 1);
                
                // Our indices are 0-based
                vertexIndex -= 1;
                texCoordIndex -= 1;
                normalIndex -= 1;

                outVertices.push_back(vertices[vertexIndex]);
                outTexCoords.push_back(texCoords[texCoordIndex]);
            }

            break;
        }
    }
}

glm::vec3 OBJLoader::readVertex(std::string line) {
    line.erase(0, line.find(' ') + 1);
    float x = std::stof(line.substr(0, line.find(' ')));
    line.erase(0, line.find(' ') + 1);
    float y = std::stof(line.substr(0, line.find(' ')));
    line.erase(0, line.find(' ') + 1);
    float z = std::stof(line.substr(0, line.find(' ')));

    return glm::vec3(x,y,z);
}

glm::vec2 OBJLoader::readTexCoord(std::string line) {
    line.erase(0, line.find(' ') + 1);
    float u = std::stof(line.substr(0, line.find(' ')));
    line.erase(0, line.find(' ') + 1);
    float v = std::stof(line.substr(0, line.find(' ')));

    return glm::vec2(u,v);
}
