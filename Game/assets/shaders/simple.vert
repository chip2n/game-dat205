#version 330 core

layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 color;

out vec3 fragmentColor;
uniform mat4 modelViewProjectionMatrix;

void main() {
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
    fragmentColor = vec3(1.0, 0.0, 0.0);
}

