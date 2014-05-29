#version 330 core
in vec2 texCoords;
in vec3 worldPosition;
in vec3 outNormal;
out vec4 color;
uniform sampler2D texSampler;
uniform vec3 worldLight;
uniform vec3 lightPos[4];

void main() {
    vec4 phong = vec4(0.0, 0.0, 0.0, 1.0);
    for(int i = 0; i < lightPos.length; i++) {
        float falloff = 1/(pow(length((lightPos[i] - worldPosition)), 1));
        if(falloff < 0.0001) break;
        vec3 lightDir = normalize(lightPos[i] - worldPosition);
        float theta = dot(outNormal, lightDir);


        vec3 diffuse = vec3(0.5, 0.5, 0.5)*theta;
        vec3 ambient = vec3(0.1, 0.1, 0.1);

        phong += vec4(falloff*(ambient+diffuse), 0.0);
    }

    vec4 c = texture(texSampler, texCoords);
    color = phong * c;
}
