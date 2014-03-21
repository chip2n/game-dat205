#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "SkinnedModel.h"

void SkinnedModel::setupBuffers() {
    copyAiMatrixToGLM(&(scene->mRootNode->mTransformation), globalInverseTransform);
    globalInverseTransform = glm::inverse(globalInverseTransform);
    numVertices = 0;
    meshEntries.resize(scene->mNumMeshes);

    for(unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];
        meshEntries[i].baseVertex = numVertices;
        numVertices += mesh->mNumVertices;

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
    bones.resize(numVertices);

    for(unsigned int i = 0; i <  scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];
        loadBones(i, mesh);
    }

    for(unsigned int i = 0; i < scene->mNumAnimations; ++i) {
        aiAnimation* anim = scene->mAnimations[i];

        std::cout << "Animation duration: " << anim->mDuration << std::endl;
        std::cout << "Animation channels: " << anim->mNumChannels << std::endl;
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






    glGenBuffers(1, &bonesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bonesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0])*bones.size(), &bones[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (void*)0);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*) 16);
}

void SkinnedModel::loadBones(uint meshIndex, const aiMesh* pMesh) {
    std::cout << "-------------------------" << std::endl;
    std::cout << "Loading bones (" << pMesh->mNumBones << ")" << std::endl;

    for(uint i = 0; i < pMesh->mNumBones; i++) {
        uint boneIndex = 0;
        std::string boneName(pMesh->mBones[i]->mName.data);
        std::cout << "Bone: " << boneName << std::endl;

        std::cout << "Searthing for " << boneName << std::endl;
        if(boneMapping.find(boneName) == boneMapping.end()) {
            std::cout << "Found " << boneName << std::endl;
            boneIndex = numBones;
            numBones++;
            BoneInfo bi;
            boneInfo.push_back(bi);
        } else {
            boneIndex = boneMapping[boneName];
        }

        boneMapping[boneName] = boneIndex;
        boneInfo[boneIndex].offsetMatrix = pMesh->mBones[i]->mOffsetMatrix;

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

void SkinnedModel::boneTransform(float timeInSeconds, std::vector<glm::mat4>& transforms) {
    glm::mat4 identity(1.0f);

    float ticksPerSecond = scene->mAnimations[0]->mTicksPerSecond != 0 ? 
                           scene->mAnimations[0]->mTicksPerSecond : 25.0f;

    float timeInTicks = timeInSeconds * ticksPerSecond;
    float animationTime = fmod(timeInTicks, scene->mAnimations[0]->mDuration);

    readNodeHierarchy(animationTime, scene->mRootNode, identity);

    transforms.resize(numBones);

    for(uint i = 0; i < numBones; i++) {
        transforms[i]= boneInfo[i].finalTransformation;
    }
}

void SkinnedModel::readNodeHierarchy(float animationTime, const aiNode* pNode, const glm::mat4 parentTransform) {
    std::string nodeName(pNode->mName.data);

    const aiAnimation* pAnimation = scene->mAnimations[0];

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
       glm::mat4 rotationM(rotationMTemp);

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

const aiNodeAnim* SkinnedModel::findNodeAnim(const aiAnimation* pAnimation, std::string &nodeName) {
    for(unsigned int i = 0; i < pAnimation->mNumChannels; ++i) {
        std::string n(pAnimation->mChannels[i]->mNodeName.data);
        if(n == nodeName) {
            return pAnimation->mChannels[i];
        }
    }
    //std::cout << "Warning: did not find " << nodeName << std::endl;
    return NULL;
}

void SkinnedModel::calcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim* pNodeAnim) {
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

void SkinnedModel::calcInterpolatedRotation(aiQuaternion &out, float animationTime, const aiNodeAnim* pNodeAnim) {
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

uint SkinnedModel::findRotation(float animationTime, const aiNodeAnim* pNodeAnim) {
    assert(pNodeAnim->mNumRotationKeys > 0);

    for(uint i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
        if(animationTime < (float)pNodeAnim->mRotationKeys[i+1].mTime) {
            return i;
        }
    }

    assert(0);
}

void SkinnedModel::calcInterpolatedPosition(aiVector3D &out, float animationTime, const aiNodeAnim* pNodeAnim) {
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

uint SkinnedModel::findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{    
    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    
    assert(0);

    return 0;
}

uint SkinnedModel::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
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

inline void SkinnedModel::copyAiMatrixToGLM(const aiMatrix4x4 *from, glm::mat4 &to)
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

inline void SkinnedModel::copyAiMatrixToGLM(const aiMatrix3x3 *from, glm::mat3 &to)
{
    to[0][0] = (GLfloat)from->a1; to[1][0] = (GLfloat)from->a2;
    to[2][0] = (GLfloat)from->a3;
    to[0][1] = (GLfloat)from->b1; to[1][1] = (GLfloat)from->b2;
    to[2][1] = (GLfloat)from->b3;
    to[0][2] = (GLfloat)from->c1; to[1][2] = (GLfloat)from->c2;
    to[2][2] = (GLfloat)from->c3; 
}
