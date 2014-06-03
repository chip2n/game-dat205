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

//vec2 poissonDisk[4] = vec2[](
//  vec2( -0.94201624, -0.39906216 ),
//  vec2( 0.94558609, -0.76890725 ),
//  vec2( -0.094184101, -0.92938870 ),
//  vec2( 0.34495938, 0.29387760 )
//);
//vec2 poissonDisk[7] = vec2[](
//  vec2(0.9781557f, 0.08678176f),
//  vec2(0.2422839f, 0.8379077f),
//  vec2(0.05205152f, 0.1375587f),
//  vec2(0.3313435f, -0.4023863f),
//  vec2(-0.6087734f, 0.06514339f),
//  vec2(-0.5291746f, 0.674904f),
//  vec2(-0.5743152f, -0.7018966f)
//);
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
    vec3 normal = outNormal;
    if(normalMapEnabled) {
        normal = vec3(texture(normalMap, texCoords));
    }
    vec4 phong = vec4(0.1, 0.1, 0.1, 1.0);
    float closestLightDistance = -1.0;
    float totalFalloff = 0.0;
    for(int i = 0; i < lights.length; i++) {
        if(lights[i].position == vec3(0)) {
            continue;
        }
        if(closestLightDistance < 0 || length(lights[i].position - worldPosition) < closestLightDistance) {
            closestLightDistance = length(lights[i].position - worldPosition);
        }

        float falloff = 1/(pow(length((lights[i].position - worldPosition)), 2));
        totalFalloff += falloff;
        if(falloff < 0.0001) {
            break;
        }
        vec3 lightDir = normalize(lights[i].position - worldPosition);
        float theta = dot(normal, lightDir);

        vec3 diffuse = vec3(0.6, 0.6, 0.6)*lights[i].color*theta;
        vec3 ambient = vec3(0.25, 0.25, 0.25);
        if(isLightSource) {
          ambient = vec3(0.6, 0.6, 0.6);
        }

        //vec4 phong = vec4(falloff*visibility*(ambient + diffuse), 1.0);
        phong += vec4(falloff*(ambient+diffuse), 0.0);
    }

    float visibility = 1.0;
    float bias = 0.00125;
    for (int i=0;i<poissonDisk.length;i++){
      if ( texture2D( shadowMap, shadowCoord.xy + poissonDisk[i]/700.0 ).z  <  shadowCoord.z-bias ){
        if(!isLightSource && receivesShadows) {
          visibility -= 0.02;
        }
      }
    }

    visibility *= min(0.8,totalFalloff);

    vec3 heightFalloff = vec3(1,1,1);
    if(worldPosition.y > 1.0 && receivesShadows) {
        heightFalloff = max(0, 1.1 - (pow(worldPosition.y - 1.0, 2))) * heightFalloff;
    }
    vec4 c = texture(texSampler, texCoords);
    color = vec4(heightFalloff, 1.0) * visibility * phong * c;

}
