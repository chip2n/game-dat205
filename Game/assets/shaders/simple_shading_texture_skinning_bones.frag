#version 330 core
in vec2 texCoords;
in vec3 worldPosition;
in vec3 outNormal;
out vec4 color;
uniform sampler2D texSampler;
uniform vec3 lightPos;

void main() {
    vec3 lightDir = normalize(lightPos - worldPosition);
    float theta = dot(outNormal, lightDir);

    vec4 c = texture(texSampler, texCoords);

    vec3 diffuse = vec3(0.5, 0.5, 0.5)*theta;
    vec3 ambient = vec3(0.5, 0.5, 0.5);

    float visibility = 1.0;
    vec4 phong = vec4((ambient + diffuse), 1.0);

    color = phong * c;
}
