#pragma once

enum class ShaderType : uint8_t
{
	Vertex,
	Fragment,
	TessellationControl,
	TessellationEvaluation,
	Geometry
};

enum class ShaderSourceType : uint8_t
{
	File,
	String
};

class Shader : Noncopyable
{
	friend class ShaderProgram;
public:
	Shader(ShaderType type, const std::string &data, ShaderSourceType sourceType = ShaderSourceType::File);
	~Shader();
	
private:
	void create(const GLchar *source[], size_t size);

	GLenum m_type;
	GLuint m_shader;
};