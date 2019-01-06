#version 400 core

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    texCoords = inTexCoord;
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
}