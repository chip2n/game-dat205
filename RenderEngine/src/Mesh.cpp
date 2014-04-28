#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"

bool Mesh::loadMesh(const string& fileName) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(NUM_VBOS, buffers);

    mScene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    bool ret = false;

    if(mScene) {
        copyAiMatrixToGLM(&(mScene->mRootNode->mTransformation), globalInverseTransform);
        globalInverseTransform = glm::inverse(globalInverseTransform);

        ret = initFromScene(mScene, fileName);
    } else {
        std::cout << "Could not load " << fileName << "." << std::endl;
        std::cout << "Error: " << importer.GetErrorString() << std::endl;
    }

    glBindVertexArray(0);

    return ret;
}

bool Mesh::initFromScene(const aiScene* scene, const string& fileName) {
    meshEntries.resize(scene->mNumMeshes);

    vector<glm::vec3> positions;
    vector<glm::vec3> normals;
    vector<glm::vec2> texCoords;
    vector<VertexBoneData> bones;
    vector<uint> indices;

    uint numVertices = 0;
    uint numIndices = 0;

    for(uint i = 0; i < meshEntries.size(); i++) {
        meshEntries[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
        meshEntries[i].numIndices = scene->mMeshes[i]->mNumFaces * 3;
        meshEntries[i].baseVertex = numVertices;
        meshEntries[i].baseIndex = numIndices;

        numVertices += scene->mMeshes[i]->mNumVertices;
        numIndices += meshEntries[i].numIndices;
    }

    positions.reserve(numVertices);
    normals.reserve(numVertices);
    texCoords.reserve(numVertices);
    bones.resize(numVertices);
    indices.reserve(numIndices);

    for(uint i = 0; i < meshEntries.size(); i++) {
        const aiMesh* paiMesh = scene->mMeshes[i];
        initMesh(i, paiMesh, positions, normals, texCoords, bones, indices);
    }

    /*
    for(int i = 0; i < bones.size(); i++) {
        std::cout << "--------------------------" << std::endl;
        std::cout << "BONEDATA" << std::endl;
        std::cout << "Weight 0: " << bones[i].Weights[0] << std::endl;
        std::cout << "Weight 1: " << bones[i].Weights[1] << std::endl;
        std::cout << "Weight 2: " << bones[i].Weights[2] << std::endl;
        std::cout << "Weight 3: " << bones[i].Weights[3] << std::endl;
    }
    */

    if(!initMaterials(scene, fileName)) {
        return false;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[POS_VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0])* positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXCOORD_VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffers[NORMAL_VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0])* normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[BONE_VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    return true;
}

bool Mesh::initMaterials(const aiScene* pScene, const string& fileName) {
    string::size_type slashIndex = fileName.find_last_of("/");
    string dir;

    if(slashIndex == string::npos) {
        dir = ".";
    } else if(slashIndex == 0) {
        dir = "/";
    } else {
        dir = fileName.substr(0, slashIndex);
    }

    bool ret = true;

    for(uint i = 0; i < pScene->mNumMaterials; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        textures.resize(pScene->mNumMaterials);
        textures[i] = NULL;

        if(pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;

            if(pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
            std::cout << "RFDG" << std::endl;
                string p(path.data);

                if(p.substr(0,2) == ".\\") {
                    p = p.substr(2, p.size() - 2);
                }

                string fullPath = dir + "/" + p;

                std::cout << "Loading texture: " << fullPath << std::endl;
                textures[i] = new Texture(fullPath);

                /*
                if(!textures[i]->load()) {
                    std::cout << "Error loading texture " << fullPath << std::endl;
                    delete textures[i];
                    textures[i] = NULL;
                    ret = false;
                } else {
                    std::cout << "Loaded texture " << fullPath << std::endl;
                }
                */
            }
        }
    }

    return ret;
}

void Mesh::initMesh(uint meshIndex,
                    const aiMesh* paiMesh,
                    vector<glm::vec3>& positions,
                    vector<glm::vec3>& normals,
                    vector<glm::vec2>& texCoords, 
                    vector<VertexBoneData>& bones,
                    vector<uint>& indices)
{
    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);
    std::cout << "Mesh vertices: " << paiMesh->mNumVertices << std::endl;

    for(uint i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zero3D;

        positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
        normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
        texCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
    }

    loadBones(meshIndex, paiMesh, bones);

    for(uint i = 0; i < paiMesh->mNumFaces; i++) {
        const aiFace& face = paiMesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

}

void Mesh::loadBones(uint meshIndex, const aiMesh* pMesh, vector<VertexBoneData>& bones) {
    std::cout << "-------------------------" << std::endl;
    std::cout << "Loading bones (" << pMesh->mNumBones << ")" << std::endl;

    for(uint i = 0; i < pMesh->mNumBones; i++) {
        uint boneIndex = 0;
        std::string boneName(pMesh->mBones[i]->mName.data);

        if(boneMapping.find(boneName) == boneMapping.end()) {
            boneIndex = numBones;
            numBones++;
            BoneInfo bi;
            boneInfo.push_back(bi);
            boneInfo[boneIndex].offsetMatrix = pMesh->mBones[i]->mOffsetMatrix;
            boneMapping[boneName] = boneIndex;
        } else {
            boneIndex = boneMapping[boneName];
        }


        for(uint j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
            uint vertexID = meshEntries[meshIndex].baseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float weight = pMesh->mBones[i]->mWeights[j].mWeight;
            bones[vertexID].addBoneData(boneIndex, weight);
        }
    }
}

void VertexBoneData::addBoneData(uint boneID, float weight) {
    for (uint i = 0 ; i <  NUM_BONES_PER_VERTEX; i++) {
        if (Weights[i] == 0.0f) {
            IDs[i] = boneID;
            Weights[i] = weight;
            return;
        }
    }

    // should never get here - more bones than we have space for
    assert(0);
}

void Mesh::boneTransform(std::string animName, float timeInSeconds, std::vector<glm::mat4>& transforms) {
    glm::mat4 identity(1.0f);

    if(animationMap.find(animName) != animationMap.end()) {
        Animation anim = animationMap[animName];

        if(mScene->mNumAnimations > 0) {
            float ticksPerSecond = mScene->mAnimations[0]->mTicksPerSecond != 0 ? 
                                   mScene->mAnimations[0]->mTicksPerSecond : 25.0f;

            float timeInTicks = timeInSeconds * ticksPerSecond;
            //float animationTime = fmod(timeInTicks, mScene->mAnimations[0]->mDuration);

            float totalDuration = mScene->mAnimations[0]->mDuration;
            float secondsPerFrame = totalDuration / anim.totalFrames;
            float startTime = anim.startFrame * secondsPerFrame;
            float endTime = anim.endFrame * secondsPerFrame;
            float duration = anim.endFrame * secondsPerFrame - startTime;

            float animationTime = fmod(timeInTicks, totalDuration - (totalDuration - endTime) - startTime);
            animationTime += startTime;


            //float animationTime = fmod(timeInTicks, anim.animDuration);
            //animationTime += anim.timePerFrame * anim.startFrame;

            readNodeHierarchy(animationTime, mScene->mRootNode, identity);

            transforms.resize(numBones);

            for(uint i = 0; i < numBones; i++) {
                transforms[i] = boneInfo[i].finalTransformation;
            }
        } else {
            std::cout << "WARNING: using bonetransform without any animations." << std::endl;
        }
    } else {
        std::cout << "Error: could not find animation " << animName << std::endl;
    }
}

void Mesh::readNodeHierarchy(float animationTime, const aiNode* pNode, const glm::mat4 parentTransform) {
    std::string nodeName(pNode->mName.data);

    const aiAnimation* pAnimation = mScene->mAnimations[0];

    // TODO: Potential bug?
    glm::mat4 nodeTransformation;
    copyAiMatrixToGLM(&(pNode->mTransformation), nodeTransformation);

   const aiNodeAnim* pNodeAnim = findNodeAnim(pAnimation, nodeName); 

   if(pNodeAnim) {
       // Interpolate scaling and generate scaling transformation matrix
       aiVector3D scaling;
       calcInterpolatedScaling(scaling, animationTime, pNodeAnim);
       glm::mat4 scalingM;
       scalingM = glm::scale(glm::mat4(1.0), glm::vec3(scaling.x, scaling.y, scaling.z));

       // Interpolate rotation and generate rotation transformation matrix
       aiQuaternion rotationQ;
       calcInterpolatedRotation(rotationQ, animationTime, pNodeAnim);
       aiMatrix3x3 rotation = rotationQ.GetMatrix();
       glm::mat3 rotationMTemp;
       copyAiMatrixToGLM(&rotation, rotationMTemp);
       glm::mat4 rotationM(rotationMTemp);  // TODO: Assuming the bottom left element is 1

       // Interpolate translation and generate translation transformation matrix
       aiVector3D translation;
       calcInterpolatedPosition(translation, animationTime, pNodeAnim);
       glm::mat4 translationM;
       translationM = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));

       // Combine the above transformations
       nodeTransformation = translationM * rotationM * scalingM;
   }

   glm::mat4 globalTransformation = parentTransform * nodeTransformation;

   if(boneMapping.find(nodeName) != boneMapping.end()) {
       uint boneIndex = boneMapping[nodeName];
       glm::mat4 offsetMatrix;
       copyAiMatrixToGLM(&(boneInfo[boneIndex].offsetMatrix), offsetMatrix);
       boneInfo[boneIndex].finalTransformation = globalInverseTransform * globalTransformation * offsetMatrix;
   }

   for(uint i = 0; i < pNode->mNumChildren; i++) {
       readNodeHierarchy(animationTime, pNode->mChildren[i], globalTransformation);
   }
}

const aiNodeAnim* Mesh::findNodeAnim(const aiAnimation* pAnimation, std::string &nodeName) {
    for(unsigned int i = 0; i < pAnimation->mNumChannels; ++i) {
        std::string n(pAnimation->mChannels[i]->mNodeName.data);
        if(n == nodeName) {
            return pAnimation->mChannels[i];
        }
    }
    //std::cout << "Warning: did not find " << nodeName << std::endl;
    return NULL;
}

void Mesh::calcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim* pNodeAnim) {
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    uint NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    //assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}

void Mesh::calcInterpolatedRotation(aiQuaternion &out, float animationTime, const aiNodeAnim* pNodeAnim) {
    // we need at least two values to interpolate
    if(pNodeAnim->mNumRotationKeys == 1) {
        out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    uint rotationIndex = findRotation(animationTime, pNodeAnim);
    uint nextRotationIndex = rotationIndex + 1;
    assert(nextRotationIndex < pNodeAnim->mNumRotationKeys);
    float deltaTime = pNodeAnim->mRotationKeys[nextRotationIndex].mTime - pNodeAnim->mRotationKeys[rotationIndex].mTime;

    float factor = (animationTime - (float) pNodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
    //assert(factor >= 0.0f && factor <= 1.0f);

    const aiQuaternion& startRotationQ = pNodeAnim->mRotationKeys[rotationIndex].mValue;
    const aiQuaternion& endRotationQ = pNodeAnim->mRotationKeys[nextRotationIndex].mValue;
    aiQuaternion::Interpolate(out, startRotationQ, endRotationQ, factor);
    out = out.Normalize();
}

uint Mesh::findRotation(float animationTime, const aiNodeAnim* pNodeAnim) {
    assert(pNodeAnim->mNumRotationKeys > 0);

    for(uint i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
        if(animationTime < (float)pNodeAnim->mRotationKeys[i+1].mTime) {
            return i;
        }
    }

    assert(0);
}

void Mesh::calcInterpolatedPosition(aiVector3D &out, float animationTime, const aiNodeAnim* pNodeAnim) {
    if (pNodeAnim->mNumPositionKeys == 1) {
        out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }
            
    uint positionIndex = findPosition(animationTime, pNodeAnim);
    uint nextPositionIndex = (positionIndex + 1);
    assert(nextPositionIndex < pNodeAnim->mNumPositionKeys);
    float deltaTime = (float)(pNodeAnim->mPositionKeys[nextPositionIndex].mTime - pNodeAnim->mPositionKeys[positionIndex].mTime);
    float factor = (animationTime - (float)pNodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
    //assert(factor >= 0.0f && factor <= 1.0f);
    const aiVector3D& start = pNodeAnim->mPositionKeys[positionIndex].mValue;
    const aiVector3D& end = pNodeAnim->mPositionKeys[nextPositionIndex].mValue;
    aiVector3D delta = end - start;
    out = start + factor * delta;
}

uint Mesh::findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{    
    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}

uint Mesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);
    
    for (uint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}

inline void Mesh::copyAiMatrixToGLM(const aiMatrix4x4 *from, glm::mat4 &to)
{
    to[0][0] = (GLfloat)from->a1; to[1][0] = (GLfloat)from->a2;
    to[2][0] = (GLfloat)from->a3; to[3][0] = (GLfloat)from->a4;
    to[0][1] = (GLfloat)from->b1; to[1][1] = (GLfloat)from->b2;
    to[2][1] = (GLfloat)from->b3; to[3][1] = (GLfloat)from->b4;
    to[0][2] = (GLfloat)from->c1; to[1][2] = (GLfloat)from->c2;
    to[2][2] = (GLfloat)from->c3; to[3][2] = (GLfloat)from->c4;
    to[0][3] = (GLfloat)from->d1; to[1][3] = (GLfloat)from->d2;
    to[2][3] = (GLfloat)from->d3; to[3][3] = (GLfloat)from->d4;
}

inline void Mesh::copyAiMatrixToGLM(const aiMatrix3x3 *from, glm::mat3 &to)
{
    to[0][0] = (GLfloat)from->a1; to[1][0] = (GLfloat)from->a2;
    to[2][0] = (GLfloat)from->a3;
    to[0][1] = (GLfloat)from->b1; to[1][1] = (GLfloat)from->b2;
    to[2][1] = (GLfloat)from->b3;
    to[0][2] = (GLfloat)from->c1; to[1][2] = (GLfloat)from->c2;
    to[2][2] = (GLfloat)from->c3; 
}

void Mesh::render(ShaderProgram &shaderProgram, Camera &camera, Environment &env, glm::vec3 position, glm::vec3 up, glm::quat rotation) {
    shaderProgram.begin();
    shaderProgram.setUniform("modelViewProjectionMatrix", camera.getCombinedMatrix());

    glm::mat4 modelM;
    modelM = glm::translate(modelM, position);
    modelM = modelM * glm::toMat4(rotation);
    shaderProgram.setUniform("modelMatrix", modelM);
    shaderProgram.setUniform("lightPos", env.getLights()[0].getPosition());


    glBindVertexArray(vao);

    for(uint i = 0; i < meshEntries.size(); i++) {
        const uint materialIndex = meshEntries[i].materialIndex;
        assert(materialIndex < textures.size());

        if(textures[materialIndex]) {
            textures[materialIndex]->bind();
        }
        
        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 meshEntries[i].numIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(uint) * meshEntries[i].baseIndex),
                                 meshEntries[i].baseVertex);
    }

    glBindVertexArray(0);

    shaderProgram.end();
}

void Mesh::addAnimation(Animation animation) {
    std::cout << "Adding animation: " << animation.name << std::endl;
    animationMap[animation.name] = animation;
}
