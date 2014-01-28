#version 330 core
in vec2 texCoords;
out vec3 color;
uniform sampler2D texSampler;

void main() {
    color = texture(texSampler, texCoords).rgb;
}
