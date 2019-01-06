#version 400 core

in vec2 texCoordsForFragments;
in vec4 colorForFragments; //not used when texture is being applied

out vec4 fColor;

uniform sampler2D beachTexture;
uniform sampler2D brickTexture;

void main()
{
    fColor = mix(texture(beachTexture, texCoordsForFragments), texture(brickTexture, texCoordsForFragments), 0.5);
} 
