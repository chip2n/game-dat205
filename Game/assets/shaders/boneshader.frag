#version 330 core
in vec2 texCoords;
in vec3 worldPosition;
in vec3 outNormal;
in vec4 shadowCoord;
out vec4 color;
uniform sampler2D texSampler;
uniform sampler2D shadowMap;
uniform vec3 worldLight;
uniform bool receivesShadows;
struct Light {
    vec3 position;
    vec3 color;
};
uniform Light lights[10];




vec2 poissonDisk[24] = vec2[](
  vec2(-0.05389525f, -0.4732464f),
  vec2(0.3748684f, -0.3568592f),
  vec2(-0.1696842f, -0.765079f),
  vec2(-0.3882916f, -0.1095552f),
  vec2(0.0610783f, -0.9829572f),
  vec2(0.07125714f, -0.1656173f),
  vec2(0.3340944f, -0.7217603f),
  vec2(-0.5315822f, -0.764813f),
  vec2(-0.5581032f, -0.3972449f),
  vec2(-0.04634873f, 0.1138161f),
  vec2(0.7350358f, -0.6742316f),
  vec2(0.808874f, -0.01892805f),
  vec2(0.3973725f, 0.1824621f),
  vec2(0.7849492f, -0.3197357f),
  vec2(-0.6595317f, 0.07930014f),
  vec2(-0.9253961f, -0.1296234f),
  vec2(-0.2353496f, 0.3982215f),
  vec2(-0.6499953f, 0.5391897f),
  vec2(0.2311781f, 0.6277929f),
  vec2(-0.2789708f, 0.7210696f),
  vec2(0.7387629f, 0.3382523f),
  vec2(0.5726824f, 0.7927272f),
  vec2(-0.8975564f, 0.3412585f),
  vec2(0.06870192f, 0.8815266f)
);

void main() {
    vec4 phong = vec4(0.1, 0.1, 0.1, 1.0);
    float totalFalloff = 0.0;
    for(int i = 0; i < lights.length; i++) {
        if(lights[i].position == vec3(0)) {
            continue;
        }
        float falloff = 1/(pow(length((lights[i].position - worldPosition)), 2));
        totalFalloff += falloff;
        if(falloff < 0.0001) break;
        vec3 lightDir = normalize(lights[i].position - worldPosition);
        float theta = dot(outNormal, lightDir);


        vec3 diffuse = vec3(0.6, 0.6, 0.6)*lights[i].color*theta;
        vec3 ambient = vec3(0.25, 0.25, 0.25);

        phong += vec4(falloff*(ambient+diffuse), 0.0);
    }

    float visibility = 1.0;
    float bias = 0.0025;
    for (int i=0;i<poissonDisk.length;i++){
      if ( texture2D( shadowMap, shadowCoord.xy + poissonDisk[i]/700.0 ).z  <  shadowCoord.z-bias ){
        if(receivesShadows) {
          visibility -= 0.02;
        }
      }
    }

    visibility *= min(0.8,totalFalloff);

    vec4 c = texture(texSampler, texCoords);
    color = phong * vec4(vec3(visibility), 1.0) * c;
}
