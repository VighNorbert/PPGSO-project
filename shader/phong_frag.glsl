#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;
uniform sampler2D ShadowMap;

struct Light {
  vec3 position;

  bool isSpotlight;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  float constant;
  float linear;
  float quadratic;

  float maxDist;

  vec3 color;
};

#define MAX_POINT_LIGHTS 32

uniform Light lights[MAX_POINT_LIGHTS];

uniform float LightsCount;

// Position of light
uniform vec3 LightPosition;

// Position of view
uniform vec3 ViewPosition;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

uniform float AmbientStrength;
uniform float DiffuseStrength;
uniform float SpecularStrength;

// The vertex shader will feed this input
in vec2 texCoord;

// Wordspace normal passed from vertex shader
in vec4 normal;

in vec3 fragmetPosition;

in vec4 fragmentPositionLightSpace;

// The final color
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

float ShadowCalculation(vec3 lightDir)
{
  // perform perspective divide
  vec3 projCoords = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;
  // transform to [0,1] range
  projCoords = projCoords * 0.5 + 0.5;
  if(projCoords.z > 1.0)
    return 0.0;
  // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
  float closestDepth = texture(ShadowMap, projCoords.xy).r;
  // get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;

  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
  for(int x = -2; x <= 2; ++x)
  {
    for(int y = -2; y <= 2; ++y)
    {
      float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
      shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
    }
  }
  shadow /= 25.0;

  return shadow;
}

vec3 CalcLight(Light light, vec3 viewDir, vec3 objectColor, bool shadowsEnabled)
{

  // attenuation
  float distance = length(light.position - fragmetPosition);

  if (light.maxDist != -1.f && light.maxDist < distance) {
    return vec3(0);
  }

  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  // light direction
  vec3 lightDir = normalize(light.position - fragmetPosition);

  float intensity = 1.0f;
  if(light.isSpotlight) {
    float theta = dot(lightDir, normalize(-light.direction));
    if (theta < light.outerCutOff)
      return vec3(0);

    float epsilon = light.cutOff - light.outerCutOff;
    intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
  }

  vec3 color = attenuation * light.color * objectColor;

  // ambient shading
  vec3 ambient = AmbientStrength * color;
  // diffuse shading
  vec3 diffuse = DiffuseStrength * max(dot(normal, vec4(lightDir, 1.0f)), 0.0f) * color;
  // specular shading
  vec3 reflectDir = reflect(-lightDir, vec3(normal));
  vec3 specular = SpecularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 32) * color;

  float shadow = shadowsEnabled ? ShadowCalculation(lightDir) : 0.f;

  return (ambient + (1.0 - shadow) * (diffuse + specular)) * intensity;
}

void main() {
  vec3 viewDir = normalize(ViewPosition - fragmetPosition);

  // Lookup the color in Texture on coordinates given by texCoord
  vec4 objectColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset);

  FragColor = vec4(0.f);
  vec3 result = vec3(0.f);
  for (int i = 0; i<LightsCount; i++) {
    result += CalcLight(lights[i], viewDir, vec3(objectColor), (i == 0));
  }
  float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
  if(brightness > 1.0) {
    BrightColor = vec4(result, 1.0);
  } else {
    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
  FragColor = vec4(result, Transparency);
}
