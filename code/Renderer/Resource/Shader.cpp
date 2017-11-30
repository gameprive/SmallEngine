// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Shader.h"

//-----------------------------------------------------------------------
GLenum toGLShaderType(ShaderType mode)
{
	switch ( mode )
	{
	case ShaderType::Vertex: return GL_VERTEX_SHADER;
	case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
	case ShaderType::TessellationControl: return GL_TESS_CONTROL_SHADER;
	case ShaderType::TessellationEvaluation: return GL_TESS_EVALUATION_SHADER;
	case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
	}
	ParameterFailed("ShaderType");
}
//-----------------------------------------------------------------------
void printShaderLogInfo(GLuint obj)
{
	int infologLength = 0;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	if ( infologLength > 1 )
	{
		char *infoLog = new char[infologLength + 1];
		glGetShaderInfoLog(obj, infologLength, 0, infoLog);
		Log(LevelLog::Error) << infoLog;
		delete infoLog;
	}
}
//-----------------------------------------------------------------------
Shader::Shader(ShaderType type, const std::string &path)
{	
	std::vector<uint8_t> data;
	readFromFile(path, data);
	if ( !data.empty())
	{
		m_type = toGLShaderType(type);
		create(data);
		checkCompilation(m_shader);
	}
	else
		Log(LevelLog::Error) << "No shader code found on file " << path;
}
//-----------------------------------------------------------------------
Shader::~Shader()
{
	glDeleteShader(m_shader);
}
//-----------------------------------------------------------------------
void Shader::readFromFile(const std::string &path, std::vector<uint8_t> &data)
{	
	if ( !FileSystem::ReadFile(path, data) )
		Log(LevelLog::Error) << "Cannot open shader file: " << path;
}
//-----------------------------------------------------------------------
void Shader::create(const std::vector<uint8_t> &data)
{
	Assert(data.size() < std::numeric_limits<int>::max());
	
	const GLchar *shaderSource[] = { reinterpret_cast<const GLchar*>(data.data()) };
	const GLint sourceLength = static_cast<GLint>(data.size());

	m_shader = glCreateShader(m_type);
	glShaderSource(m_shader, 1, shaderSource, &sourceLength);
	glCompileShader(m_shader);
}
//-----------------------------------------------------------------------
void Shader::checkCompilation(const GLuint shader) const
{
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if ( !result )
	{
		Log(LevelLog::Error) << "Shaders could not compile shader.";
		printShaderLogInfo(shader);
	}
}
//-----------------------------------------------------------------------