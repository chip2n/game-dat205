#ifndef MESH_H
#define MESH_H
#include <GL/glew.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <map>

#include "Animation.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Environment.h"
#include "Light.h"

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
    unsigned int materialIndex = 0;
};

class Mesh {
  public:
    // mesh loading
    bool loadFromFile(const string& fileName);

    // animation methods
    void addAnimation(Animation animation);
    void boneTransform(std::string animName,
                       float timeInSeconds,
                       std::vector<glm::mat4>& transforms);

    // render methods
    void render(ShaderProgram &shaderProgram,
                Camera &camera,
                Environment &env,
                glm::vec3 position,
                glm::vec3 up,
                glm::quat rotation);
    void render(ShaderProgram &shaderProgram,
                Camera &camera,
                Environment &env);
    void render(ShaderProgram &shaderProgram,
                Camera &camera,
                Environment &env,
                glm::vec3 position);
    void render(ShaderProgram &shaderProgram);

  private:
    void renderMesh(ShaderProgram &shaderProgram,
                    glm::vec3 position = glm::vec3(0),
                    glm::quat rotation = glm::quat());
    void shaderSetupLights(ShaderProgram &shaderProgram,
                           std::vector<Light> lights);
    bool initFromScene(const aiScene* scene, const string& fileName);
    void initMesh(uint meshIndex, const aiMesh* paiMesh);
    void initBones(uint meshIndex, const aiMesh* pMesh, vector<VertexBoneData>& bones);
    bool initMaterials(const aiScene* pScene, const string& fileName);
    bool initLights(const aiScene* pScene, const string& fileName);
    void readNodeHierarchy(float animationTime, const aiNode* pNode, const glm::mat4 parentTransform);
    void calcInterpolatedScaling(aiVector3D &scaling, float animationTime, const aiNodeAnim* pNodeAnim);
    void calcInterpolatedRotation(aiQuaternion &out, float animationTime, const aiNodeAnim* pNodeAnim);
    void calcInterpolatedPosition(aiVector3D &out, float animationTime, const aiNodeAnim* pNodeAnim);
    uint findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint findRotation(float animationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, std::string &nodeName);

    Assimp::Importer importer;
    const aiScene* mScene;
    GLuint bonesVBO;
    GLuint vao;
    uint numBones = 0;
    std::map<std::string, uint> boneMapping;
    std::map<std::string, Animation> animationMap;
    std::vector<BoneInfo> boneInfo;
    std::vector<MeshEntry> meshEntries;
    std::vector<Light> lights;
    glm::mat4 globalInverseTransform;
    GLuint buffers[NUM_VBOS];
    std::vector<Texture*> textures;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<VertexBoneData> bones;
    std::vector<uint> indices;

    // utility methods
    void copyAiMatrixToGLM(const aiMatrix4x4 *from, glm::mat4 &to);
    void copyAiMatrixToGLM(const aiMatrix3x3 *from, glm::mat3 &to);

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
