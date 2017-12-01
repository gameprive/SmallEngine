#pragma once

#include "Shader.h"

class ShaderProgram : Noncopyable
{
public:
	ShaderProgram(const std::vector<std::shared_ptr<Shader>> &shaders);
	ShaderProgram(const std::string &vertexFilePath, const std::string &fragmentFilePath);
	~ShaderProgram();

	void Bind() const;

private:
	void init(const std::vector<std::shared_ptr<Shader>> &shaders);
	void checkLinking(const GLuint program) const;
	GLuint m_program;
};