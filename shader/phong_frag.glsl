#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

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

// The final color
out vec4 FragmentColor;

vec3 CalcLight(Light light, vec3 viewDir, vec3 objectColor)
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

  return (ambient + diffuse + specular) * intensity;
}

void main() {
  vec3 viewDir = normalize(ViewPosition - fragmetPosition);

  // Lookup the color in Texture on coordinates given by texCoord
  vec4 objectColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset);

  FragmentColor = vec4(0.f);
  for (int i = 0; i<LightsCount; i++) {
    FragmentColor += vec4(CalcLight(lights[i], viewDir, vec3(objectColor)), 0.f);
  }
  FragmentColor[3] = Transparency;
}
