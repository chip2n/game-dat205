#version 330 core
in vec2 texCoords;
in vec3 worldPosition;
in vec3 outNormal;
in vec4 shadowCoord;
out vec4 color;
uniform sampler2D texSampler;
uniform sampler2D shadowMap;
uniform vec3 lightPos[5];

void main() {
    vec4 phong = vec4(0.0, 0.0, 0.0, 1.0);
    for(int i = 0; i < lightPos.length; i++) {
        float falloff = 1/(pow(length((lightPos[i] - worldPosition)), 1));
        if(falloff < 0.0001) break;
        vec3 lightDir = normalize(lightPos[i] - worldPosition);
        float theta = dot(outNormal, lightDir);


        vec3 diffuse = vec3(0.5, 0.5, 0.5)*theta;
        vec3 ambient = vec3(0.1, 0.1, 0.1);

        float visibility = 1.0;
        float bias = 0.005;
        if( texture(shadowMap, shadowCoord.xy).z < shadowCoord.z - bias) {
            visibility = 0.1;
        }
        //vec4 phong = vec4(falloff*visibility*(ambient + diffuse), 1.0);
        phong += vec4(falloff*visibility*(ambient+diffuse), 0.0);
    }

    vec4 c = texture(texSampler, texCoords);
    color = phong * c;
}
