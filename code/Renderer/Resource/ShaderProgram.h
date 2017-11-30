#pragma once

#include "Shader.h"

class ShaderProgram : Noncopyable
{
public:
	ShaderProgram(const std::vector<std::shared_ptr<Shader>> &shaders);
	~ShaderProgram();

	void Bind() const;

private:
	void checkLinking(const GLuint program) const;
	GLuint m_program;
};