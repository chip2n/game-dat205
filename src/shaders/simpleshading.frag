#version 330 core
in vec2 texCoords;
in vec3 worldPosition;
in vec3 outNormal;
in vec4 shadowCoord;
out vec4 color;
uniform sampler2D texSampler;
uniform sampler2D shadowMap;
uniform vec3 lightPos;

void main() {
    vec3 lightDir = normalize(lightPos - worldPosition);
    float theta = dot(outNormal, lightDir);

    vec4 c = texture(texSampler, texCoords);

    vec3 diffuse = vec3(0.5, 0.5, 0.5)*theta;
    vec3 ambient = vec3(0.1, 0.1, 0.1);

    float visibility = 1.0;
    float bias = 0.005;
    if( texture(shadowMap, shadowCoord.xy).z < shadowCoord.z - bias) {
        visibility = 0.1;
    }
    vec4 phong = vec4(visibility*(ambient + diffuse), 1.0);

    color = phong * c;
}
