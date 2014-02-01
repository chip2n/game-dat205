#version 330 core
in vec2 texCoords;
in vec3 worldPosition;
in vec3 outNormal;
out vec3 color;
uniform sampler2D texSampler;
uniform vec3 lightPos;

void main() {
    vec3 lightDir = normalize(lightPos - worldPosition);
    float theta = dot(outNormal, lightDir);

    vec3 c = texture(texSampler, texCoords).rgb;

    vec3 diffuse = vec3(0.5, 0.5, 0.5)*theta;
    vec3 ambient = vec3(0.1, 0.1, 0.1);
//color = (ambient + diffuse) * c;
    color = c;
}
