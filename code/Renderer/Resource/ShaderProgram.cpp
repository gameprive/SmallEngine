// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "ShaderProgram.h"

//--------------------------------------------------------------------
// glProgramUniform требует OGL 4.1
// glUniform - требует бинда (glUseProgram) перед использованием
// но так как я все равно должен биндить ShaderProgram, то возможно glUniform будет лучшим решением
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
void ShaderProgram::Uniform1(const char *name, int val)
{
	glUniform1i(getUniLoc(name), val);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform1(const char *name, float val)
{
	glUniform1f(getUniLoc(name), val);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform2(const char *name, float v1, float v2)
{
	glUniform2f(getUniLoc(name), v1, v2);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform2(const char *name, const glm::vec2 &v)
{
	glUniform2f(getUniLoc(name), v.x, v.y);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform3(const char *name, float v1, float v2, float v3)
{
	glUniform3f(getUniLoc(name), v1, v2, v3);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform3(const char *name, const glm::vec3 &v)
{
	glUniform3f(getUniLoc(name), v.x, v.y, v.z);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform4(const char *name, float v1, float v2, float v3, float v4)
{
	glUniform4f(getUniLoc(name), v1, v2, v3, v4);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform4(const char *name, const glm::vec4 &v)
{
	glUniform4f(getUniLoc(name), v.x, v.y, v.z, v.w);
}
//--------------------------------------------------------------------
void ShaderProgram::UniformMatrix3(const char *name, int count, const float *vals, bool transpose)
{
	glUniformMatrix3fv(getUniLoc(name), count, transpose, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::UniformMatrix4(const char *name, int count, const float *vals, bool transpose)
{
	glUniformMatrix4fv(getUniLoc(name), count, transpose, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform1v(const char *name, int count, const float *vals)
{
	glUniform1fv(getUniLoc(name), count, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform2v(const char *name, int count, const float *vals)
{
	glUniform2fv(getUniLoc(name), count, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform3v(const char *name, int count, const float *vals)
{
	glUniform3fv(getUniLoc(name), count, vals);
}
//--------------------------------------------------------------------
void ShaderProgram::Uniform4v(const char *name, int count, const float *vals)
{
	glUniform4fv(getUniLoc(name), count, vals);
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

	getUniformLocations();
}
//--------------------------------------------------------------------
void ShaderProgram::checkLinking(const GLuint program) const
{
	GLint linked = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
	if ( !linked )
		LogError() << "Shaders compiled but could not be linked";

	GLint bufflen;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufflen);
	if ( bufflen > 1 )
	{
		GLchar *log = new char[bufflen + 1];
		glGetProgramInfoLog(program, bufflen, 0, log);
		LogError() << "Linking log:\n" << log;
		delete log;
	}
}
//--------------------------------------------------------------------
GLint ShaderProgram::getUniLoc(const char *name)
{
	// что быстрее - glGetUniformLocation или доступ к хешмапе по строке?
	// выявлена ошибка при работе с хешмапой
#if 1
	const GLint loc = glGetUniformLocation(m_program, name);
#else
	const GLint loc = m_uniformLocations[name];
#endif
	if ( loc == -1 ) LogError() << "uniform '" << name << "' has not been defined";
	return loc;
}
//--------------------------------------------------------------------
void ShaderProgram::getUniformLocations()
{
	int numUni = -1;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &numUni);
	
	for ( int i = 0; i < numUni; ++i )
	{
		int namelen;
		int num;
		GLenum type;
		char name[128];
		glGetActiveUniform(m_program, static_cast<GLuint>(i), sizeof(name) - 1, &namelen, &num, &type, name);
		name[namelen] = 0;		
		m_uniformLocations[name] = glGetUniformLocation(m_program, name);
	}
}
//--------------------------------------------------------------------