#version 330 core
in vec2 texCoords;
in vec3 worldPosition;
in vec3 outNormal;
in vec4 shadowCoord;
out vec4 color;
uniform sampler2D texSampler;
uniform sampler2D shadowMap;
uniform sampler2D normalMap;
uniform bool normalMapEnabled;
uniform bool receivesShadows;
uniform bool isLightSource;

struct Light {
    vec3 position;
    vec3 color;
};
uniform Light lights[10];

vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

void main() {
    vec3 normal = outNormal;
    if(normalMapEnabled) {
        normal = vec3(texture(normalMap, texCoords));
    }
    vec4 phong = vec4(0.1, 0.1, 0.1, 1.0);
    float closestLightDistance = -1.0;
    for(int i = 0; i < lights.length; i++) {
        if(lights[i].position == vec3(0)) {
            continue;
        }
        if(closestLightDistance < 0 || length(lights[i].position - worldPosition) < closestLightDistance) {
            closestLightDistance = length(lights[i].position - worldPosition);
        }

        float falloff = 1/(pow(length((lights[i].position - worldPosition)), 2));
        if(falloff < 0.0001) {
            break;
        }
        vec3 lightDir = normalize(lights[i].position - worldPosition);
        float theta = dot(normal, lightDir);

        vec3 diffuse = vec3(0.5, 0.5, 0.5)*lights[i].color*theta;
        vec3 ambient = vec3(0.2, 0.2, 0.2);
        if(isLightSource) {
          ambient = vec3(0.6, 0.6, 0.6);
        }

        //vec4 phong = vec4(falloff*visibility*(ambient + diffuse), 1.0);
        phong += vec4(falloff*(ambient+diffuse), 0.0);
    }

    float visibility = 1.0;
    float bias = 0.0025;
    for (int i=0;i<4;i++){
      if ( texture2D( shadowMap, shadowCoord.xy + poissonDisk[i]/700.0 ).z  <  shadowCoord.z-bias ){
        if(!isLightSource && receivesShadows) {
          visibility -= 0.1;
        }
      }
    }

    vec3 heightFalloff = vec3(1,1,1);
    if(worldPosition.y > 1.0 && receivesShadows) {
        heightFalloff = max(0, 1.1 - (pow(worldPosition.y - 1.0, 2))) * heightFalloff;
    }
    vec4 c = texture(texSampler, texCoords);
    color = vec4(heightFalloff, 1.0) * visibility * phong * c;

}
