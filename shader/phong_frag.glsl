#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Position of light
uniform vec3 LightPosition;

// Position of view
uniform vec3 ViewPosition;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

uniform float DiffuseStrength;
uniform float AmbientStrength;
uniform float SpecularStrength;

// The vertex shader will feed this input
in vec2 texCoord;

// Wordspace normal passed from vertex shader
in vec4 normal;

in vec3 fragmetPosition;

// The final color
out vec4 FragmentColor;

void main() {
  vec3 lightDir = normalize(LightPosition - fragmetPosition);

  // calculate diffuse
  float diffuse = max(dot(normal, vec4(lightDir, 1.0f)), 0.0f) * DiffuseStrength;

  // calculate ambient
  float ambient = AmbientStrength;

  // calculate specular
  vec3 viewDir = normalize(ViewPosition - fragmetPosition);
  vec3 reflectDir = reflect(-lightDir, vec3(normal));
  float specular = pow(max(dot(viewDir, reflectDir), 0.0f), 32) * SpecularStrength;

  float light = diffuse + ambient + specular;

  // Lookup the color in Texture on coordinates given by texCoord
  vec4 objectColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset);

  FragmentColor = objectColor * light;
  FragmentColor[3] = Transparency;
}
