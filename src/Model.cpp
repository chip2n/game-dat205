#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include "Model.h"

bool Model::loadFromFile(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

    if(!scene) {
        std::cout << "WFEGFD" << std::endl;
        return false;
    }

    std::cout << "Loaded model " << path << std::endl;
    std::cout << "-- Number of meshes: " << scene->mNumMeshes << std::endl;
    std::cout << "-- Number of animations: " << scene->mNumAnimations << std::endl;
    std::cout << "-- Number of cameras: " << scene->mNumCameras << std::endl;
    std::cout << "-- Number of lights: " << scene->mNumLights << std::endl;
    std::cout << "-- Number of materials: " << scene->mNumMaterials << std::endl;
    std::cout << "-- Number of textures: " << scene->mNumTextures << std::endl;

    for(unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];

        for(unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            const aiFace& face = mesh->mFaces[j];

            for(int k = 0; k < 3; ++k) {
                aiVector3D pos = mesh->mVertices[face.mIndices[k]];
                aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
                aiVector3D normal = mesh->HasNormals()
                    ? mesh->mNormals[face.mIndices[k]]
                    : aiVector3D(1.0f, 1.0f, 1.0f);

                positions.push_back(glm::vec3(pos.x, pos.y, pos.z));
                normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
                texCoords.push_back(glm::vec2(uv.x, uv.y));

                test.push_back(pos.x);
                test.push_back(pos.y);
                test.push_back(pos.z);
                test.push_back(uv.x);
                test.push_back(uv.y);
                test.push_back(normal.x);
                test.push_back(normal.y);
                test.push_back(normal.z);

            }
        }
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, test.size()*sizeof(float), &test[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) (3*sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) (5*sizeof(float)));

    return true;
}

unsigned int Model::getNumberOfVertices() {
    return positions.size() * 3;
}
