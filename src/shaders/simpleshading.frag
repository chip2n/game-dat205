#version 330 core
in vec2 texCoords;
in vec3 worldPosition;
in vec3 outNormal;
out vec3 color;
uniform sampler2D texSampler;
uniform vec3 lightPos;

void main() {
    vec3 l = vec3(10, 0, 0);
    vec3 lightDir = normalize(l - worldPosition);
    float theta = dot(outNormal, lightDir);

    vec3 c = texture(texSampler, texCoords).rgb;

    color = (vec3(0.1, 0.1, 0.1) + theta) * 10 * c;
}
