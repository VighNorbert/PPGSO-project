#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;

void main()
{
  vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
  if(hdr)
  {
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    FragColor = vec4(result, 1.0);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
      BrightColor = vec4(FragColor.rgb, 1.0);
  }
  else
  {
    FragColor = vec4(hdrColor, 1.0);
    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
}