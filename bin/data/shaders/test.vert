#version 330 core

layout(location = 0) in vec3 inVertex;
layout(location = 1) in vec2 inTexCoord;

uniform mat4 MVP;

out vec2 TexCoord;

void main()
{
	gl_Position = MVP * vec4(inVertex, 1.0);
	TexCoord = inTexCoord;
}