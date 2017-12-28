// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "ShaderProgram.h"

//--------------------------------------------------------------------
ShaderProgram::ShaderProgram(const std::vector<std::shared_ptr<Shader>> &shaders)
{
	init(shaders);
}
//--------------------------------------------------------------------
ShaderProgram::ShaderProgram(const std::string &vsPath, const std::string &fsPath)
{
	const std::vector<std::shared_ptr<Shader>> shaders =
	{
		std::make_shared<Shader>(ShaderType::Vertex, vsPath),
		std::make_shared<Shader>(ShaderType::Fragment, fsPath),
	};
	init(shaders);
}
//--------------------------------------------------------------------
ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_program);
}
//--------------------------------------------------------------------
void ShaderProgram::Bind() const
{
	glUseProgram(m_program);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform1f(const char *var, float val)
{
	glUniform1f(getUniLoc(var), val);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform1i(const char *var, int val)
{
	glUniform1i(getUniLoc(var), val);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform1fv(const char *var, int count, float *vals)
{
	glUniform1fv(getUniLoc(var), count, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform2f(const char *var, float v1, float v2)
{
	glUniform2f(getUniLoc(var), v1, v2);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform2fv(const char *var, int count, float *vals)
{
	glUniform2fv(getUniLoc(var), count, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::UniformVec2(const char *var, const glm::vec2 &v)
{
	Uniform2f(var, v.x, v.y);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform3iv(const char *var, int count, int *vals)
{
	glUniform3iv(getUniLoc(var), count, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform3fv(const char *var, int count, float *vals)
{
	glUniform3fv(getUniLoc(var), count, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform3f(const char *var, const float v1, const float v2, const float v3)
{
	glUniform3f(getUniLoc(var), v1, v2, v3);
}
//--------------------------------------------------------------------
void ShaderProgram::UniformVec3(const char *var, const glm::vec3 &v)
{
	Uniform3f(var, v.x, v.y, v.z);
}
//--------------------------------------------------------------------
void ShaderProgram::UniformMatrix3fv(const char *var, int count, const GLfloat *vals, bool transpose)
{
	glUniformMatrix3fv(getUniLoc(var), count, transpose, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform4iv(const char *var, int count, int *vals)
{
	glUniform4iv(getUniLoc(var), count, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform4fv(const char *var, int count, float *vals)
{
	glUniform4fv(getUniLoc(var), count, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform4f(const char *var, float v1, float v2, float v3, float v4)
{
	glUniform4f(getUniLoc(var), v1, v2, v3, v4);
}
//--------------------------------------------------------------------
void ShaderProgram::UniformVec4(const char *var, const glm::vec4 &v)
{
	Uniform4f(var, v.x, v.y, v.z, v.w);
}
//--------------------------------------------------------------------
void ShaderProgram::UniformMatrix4fv(const char *var, int count, const GLfloat *vals, bool transpose)
{
	glUniformMatrix4fv(getUniLoc(var), count, transpose, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::init(const std::vector<std::shared_ptr<Shader>> &shaders)
{
	m_program = glCreateProgram();

	for ( auto &s : shaders )
		glAttachShader(m_program, s->m_shader);

	glLinkProgram(m_program);
	checkLinking(m_program);

	for ( auto &s : shaders )
		glDetachShader(m_program, s->m_shader);
}
//--------------------------------------------------------------------
void ShaderProgram::checkLinking(const GLuint program) const
{
	GLint linked = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
	if ( !linked )
		Log(LevelLog::Error) << "Shaders::Shaders() compiled but could not be linked";

	GLint bufflen;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufflen);
	if ( bufflen > 1 )
	{
		GLchar *log = new char[bufflen + 1];
		glGetProgramInfoLog(program, bufflen, 0, log);
		Log(LevelLog::Error) << "Linking log:\n" << log;
		delete log;
	}
}
//--------------------------------------------------------------------
GLint ShaderProgram::getUniLoc(const char *name)
{
	const GLint loc = glGetUniformLocation(m_program, name);
	if ( loc == -1 )
		Log(LevelLog::Error) << "Shaders::getUniLoc(): uniform '" << name << "' has not been defined";
	return loc;
}
//--------------------------------------------------------------------
