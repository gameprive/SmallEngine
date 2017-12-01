#version 330 core

in vec2 TexCoord;

uniform sampler2D myTextureSampler;

out vec3 outColor;

void main()
{
	outColor = texture(myTextureSampler, TexCoord).rgb;
}