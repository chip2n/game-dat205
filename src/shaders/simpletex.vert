#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoordsIn;
//layout(location = 1) in vec3 color;

out vec2 texCoords;
uniform mat4 modelViewProjectionMatrix;

void main() {
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
    texCoords = texCoordsIn;
}

