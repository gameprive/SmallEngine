// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "Shader.h"

//--------------------------------------------------------------------
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
//--------------------------------------------------------------------
Shader::Shader(ShaderType type, const std::string &data, ShaderSourceType sourceType)
{
	m_type = toGLShaderType(type);

	if ( sourceType == ShaderSourceType::File )
	{
		FileData fData(data);
		if ( fData.isLoaded )
		{
			const GLchar *shaderSource[] = { reinterpret_cast<const GLchar*>(fData.data.data()) };
			create(shaderSource, fData.data.size());
		}
		else
			LogError() << "Cannot open shader file: " << data;
	}
	else if ( sourceType == ShaderSourceType::String )
	{
		if ( !data.empty() )
		{
			const GLchar *shaderSource[] = { reinterpret_cast<const GLchar*>(data.data()) };
			create(shaderSource, data.size());
		}
		else
			LogError() << "Shader data empty!!!";		
	}
}
//--------------------------------------------------------------------
Shader::~Shader()
{
	glDeleteShader(m_shader);
}
//--------------------------------------------------------------------
void Shader::create(const GLchar *source[], size_t size)
{
	const int sourceLength = static_cast<int>(size);
	Assert(sourceLength < std::numeric_limits<int>::max());

	m_shader = glCreateShader(m_type);
	glShaderSource(m_shader, 1, source, &sourceLength);
	glCompileShader(m_shader);

	GLint compileSuccess;
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &compileSuccess);
	if ( !compileSuccess )
	{
		LogError() << "Shaders could not compile shader.";

		int infologLength = 0;
		glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &infologLength);
		if ( infologLength > 0 )
		{
			char *infoLog = new char[infologLength + 1];
			glGetShaderInfoLog(m_shader, infologLength, 0, infoLog);
			LogError() << infoLog;
			delete infoLog;
		}
	}
}
//--------------------------------------------------------------------