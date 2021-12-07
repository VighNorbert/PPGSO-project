#version 330 core
layout (location = 0) in vec3 Position;

uniform mat4 LightProjectionMatrix;
uniform mat4 LightViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
    gl_Position = LightProjectionMatrix * LightViewMatrix * ModelMatrix * vec4(Position, 1.0);
}