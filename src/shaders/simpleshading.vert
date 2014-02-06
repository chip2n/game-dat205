#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoordsIn;
layout(location = 2) in vec3 normal;

//layout(location = 1) in vec3 color;

out vec2 texCoords;
out vec3 worldPosition;
out vec3 outNormal;
out vec4 shadowCoord;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 depthBiasMVP;

void main() {
    gl_Position = modelViewProjectionMatrix * modelMatrix * vec4(position, 1.0);
    worldPosition = vec3(modelMatrix * vec4(position, 1.0));
    outNormal = vec3(modelMatrix * vec4(normal, 0.0));
    texCoords = texCoordsIn;
    shadowCoord = depthBiasMVP * vec4(position, 1.0);
}

