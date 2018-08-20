#version 400 core

in vec3 position;
in vec2 texCoords;
in vec4 vertColor;

in vec3 cubePosition;
in vec2 cubeTexCoords;

out vec2 texCoordsForFragments;
out vec4 colorForFragments;

uniform mat4 translationMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    texCoordsForFragments = cubeTexCoords;
    //colorForFragments = vertColor; // Does not matter since it is not used in fragment shader
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(cubePosition, 1.0); // see how we directly give a vec3 to vec4's constructor
}