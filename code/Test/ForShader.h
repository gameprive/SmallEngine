#pragma once

#include <iostream>

class ForShader
{
public:
	static const GLchar* readShader(const char* filename);

	static GLuint makeProgram(const char* vertex, const char* fragment);
};
