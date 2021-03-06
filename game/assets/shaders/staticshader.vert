#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoordsIn;
layout(location = 2) in vec3 normal;
layout(location = 3) in ivec4 boneIDs;
layout(location = 4) in vec4 weights;



out vec2 texCoords;
out vec3 worldPosition;
out vec3 outNormal;
out vec4 shadowCoord;

const int MAX_BONES = 100;


uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 depthBiasMVP;

uniform mat4 gBones[MAX_BONES];

uniform bool useBones = false;


void main() {
    mat4 boneTransform = mat4(1.0);
    if(useBones) {
        boneTransform = gBones[boneIDs[0]] * weights[0];
        boneTransform += gBones[boneIDs[1]] * weights[1];
        boneTransform += gBones[boneIDs[2]] * weights[2];
        boneTransform += gBones[boneIDs[3]] * weights[3];
    }

    vec4 pos = boneTransform * vec4(position, 1.0);
    gl_Position = modelViewProjectionMatrix * modelMatrix * pos;
    worldPosition = vec3(modelMatrix * pos);
    outNormal = vec3(modelMatrix * vec4(normal, 0.0));
    texCoords = texCoordsIn;
    shadowCoord = depthBiasMVP * pos;
}

