#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <glm/glm.hpp>
#include "OBJLoader.h"

std::vector<glm::vec3> OBJLoader::loadOBJ(std::string path) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> outVertices;

    std::ifstream file(path.c_str());

    std::string line;
    while(std::getline(file, line)) {
        switch(line[0]) {
        case 'v':
            if(line[1] == ' ')
                vertices.push_back(readVertex(line));
            break;
        case 'f':
            line.erase(0, line.find(' ') + 1);

            std::cout << line << std::endl;
            for(int i = 0; i < 3; i++) {
                std::string currentVertex = line.substr(0, line.find(' '));
                int vertexIndex = std::stoi(currentVertex.substr(0, currentVertex.find('/'))) - 1;
                std::cout << vertexIndex << std::endl;
                line.erase(0, line.find(' ') + 1);
                outVertices.push_back(vertices[vertexIndex]);
            }

            break;
        }
    }
    std::cout << outVertices.size() << std::endl;

    return outVertices;
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
