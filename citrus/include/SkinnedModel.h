#ifndef SKINNED_MODEL_H
#define SKINNED_MODEL_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <map>
#include "Model.h"

#define NUM_BONES_PER_VERTEX 4

struct VertexBoneData {
    uint IDs[NUM_BONES_PER_VERTEX];
    float Weights[NUM_BONES_PER_VERTEX];
    void addBoneData(uint boneID, float weight);
};

struct BoneInfo {
    aiMatrix4x4 offsetMatrix;
    glm::mat4 finalTransformation;
};

struct MeshEntry {
    uint baseVertex = 0;
};
class SkinnedModel : public Model {
    public:
        virtual void setupBuffers();
        void loadBones(uint meshIndex, const aiMesh* pMesh);
        GLuint bonesVBO;
        std::vector<VertexBoneData> bones;
        std::map<std::string, uint> boneMapping;
        std::vector<BoneInfo> boneInfo;
        uint numVertices;
        std::vector<MeshEntry> meshEntries;
        void boneTransform(float timeInSeconds, std::vector<glm::mat4>& transforms);
    private:
        uint numBones = 0;
        void readNodeHierarchy(float animationTime, const aiNode* pNode, const glm::mat4 parentTransform);
        void copyAiMatrixToGLM(const aiMatrix4x4 *from, glm::mat4 &to);
        void copyAiMatrixToGLM(const aiMatrix3x3 *from, glm::mat3 &to);
        void calcInterpolatedScaling(aiVector3D &scaling, float animationTime, const aiNodeAnim* pNodeAnim);
        void calcInterpolatedRotation(aiQuaternion &out, float animationTime, const aiNodeAnim* pNodeAnim);
        void calcInterpolatedPosition(aiVector3D &out, float animationTime, const aiNodeAnim* pNodeAnim);
        uint findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
        uint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
        const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, std::string &nodeName);
        glm::mat4 globalInverseTransform;
        uint findRotation(float animationTime, const aiNodeAnim* pNodeAnim);

};


#endif
