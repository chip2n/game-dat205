#ifndef MESH_H
#define MESH_H
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <map>

#define NUM_BONES_PER_VERTEX 4
#define NUM_VBOS 5

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2
#define BONE_ID_LOCATION 3
#define BONE_WEIGHT_LOCATION 4

using namespace std;

struct VertexBoneData {
    uint IDs[NUM_BONES_PER_VERTEX];
    float Weights[NUM_BONES_PER_VERTEX];
    void addBoneData(uint boneID, float weight);
};

struct BoneInfo {
    aiMatrix4x4 offsetMatrix;
    glm::mat4 finalTransformation;

    BoneInfo() {
        offsetMatrix = aiMatrix4x4(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f);
        finalTransformation = glm::mat4(0.0f);
    }
};

struct MeshEntry {
    unsigned int numIndices = 0;
    unsigned int baseVertex = 0;
    unsigned int baseIndex = 0;
    unsigned int materialIndex = 0; // TODO: INVALID_MATERIAL

};

class Mesh {
    public:
        bool loadMesh(const string& fileName);
        bool initFromScene(const aiScene* scene, const string& fileName);
        void initMesh(uint meshIndex, const aiMesh* paiMesh, vector<glm::vec3>& positions, vector<glm::vec3>& normals, vector<glm::vec2>& texCoords,  vector<VertexBoneData>& bones, vector<uint>& indices);
        void loadBones(uint meshIndex, const aiMesh* pMesh, vector<VertexBoneData>& bones);
        GLuint bonesVBO;
        std::map<std::string, uint> boneMapping;
        std::vector<BoneInfo> boneInfo;
        std::vector<MeshEntry> meshEntries;
        void boneTransform(float timeInSeconds, std::vector<glm::mat4>& transforms);
        void render();
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

        GLuint vao;
        GLuint buffers[NUM_VBOS];

        const aiScene* mScene;
        Assimp::Importer importer;

        enum BUFFER_TYPES {
            INDEX_BUFFER,
            POS_VBO,
            NORMAL_VBO,
            TEXCOORD_VBO,
            BONE_VBO,
            NUM_VBO
        };
};


#endif
