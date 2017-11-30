﻿#pragma once

enum class ShaderType : uint8_t
{
	Vertex,
	Fragment,
	TessellationControl,
	TessellationEvaluation,
	Geometry
};

class Shader : Noncopyable
{
public:
	Shader(ShaderType type, const std::string &path);
	~Shader();

	GLuint GetGlIdentifier() const { return m_shader; }

private:
	void readFromFile(const std::string &path, std::vector<uint8_t> &data);
	void create(const std::vector<uint8_t> &data);
	void checkCompilation(const GLuint shader) const;

	GLenum m_type;
	GLuint m_shader;
};