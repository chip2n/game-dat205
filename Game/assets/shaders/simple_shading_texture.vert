#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoordsIn;
layout(location = 2) in vec3 normal;

out vec2 texCoords;
out vec3 worldPosition;
out vec3 outNormal;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

void main() {
    gl_Position = modelViewProjectionMatrix * modelMatrix * vec4(position, 1.0);
    worldPosition = vec3(modelMatrix * vec4(position, 1.0));
    outNormal = vec3(modelMatrix * vec4(normal, 0.0));
    texCoords = texCoordsIn;
}

