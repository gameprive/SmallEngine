#version 330 core

layout(location = 0) in vec3 inVertex;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat3 uNormal;

out vec2 TexCoord;
out vec3 Normal;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(inVertex, 1.0);
    Normal = normalize(uNormal * inNormal);
    TexCoord = inTexCoord;
}