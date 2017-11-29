#include "stdafx.h"
#include "Shaders.h"

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
	std::string code;
	readFromFile(path, code);
	if ( !code.empty() )
	{
		m_type = type;
		m_shader = glCreateShader(getGlShaderType(m_type));
		const char *c_str = code.c_str();
		glShaderSource(m_shader, 1, &c_str, nullptr);
		glCompileShader(m_shader);
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
void Shader::readFromFile(const std::string &path, std::string &out) const
{
	std::ifstream file;
	file.open(path);

	std::string str;
	if ( file.is_open() )
	{
		while ( std::getline(file, str) )
			out += str + '\n';
		file.close();
	}
	else
		Log(LevelLog::Error) << "Cannot open shader file: " << path;
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
GLenum Shader::getGlShaderType(ShaderType type) const
{
	switch ( type )
	{
	case VERTEX: return GL_VERTEX_SHADER;
	case FRAGMENT: return GL_FRAGMENT_SHADER;
	case TESSELLATION_CONTROL: return GL_TESS_CONTROL_SHADER;
	case TESSELLATION_EVALUATION: return GL_TESS_EVALUATION_SHADER;
	case GEOMETRY: return GL_GEOMETRY_SHADER;
	default: return 0;
	}
}
//-----------------------------------------------------------------------
ShaderProgram::ShaderProgram(const std::vector<Shader*> &shaders)
	: m_attachShader(shaders)
{
	m_program = glCreateProgram();
	for ( Shader *s : m_attachShader )
		glAttachShader(m_program, s->GetGlIdentifier());
	glLinkProgram(m_program);
	checkLinking(m_program);
}
//-----------------------------------------------------------------------
ShaderProgram::~ShaderProgram()
{
	for ( Shader *s : m_attachShader )
		glDetachShader(m_program, s->GetGlIdentifier());
	glDeleteProgram(m_program);
}
//-----------------------------------------------------------------------
void ShaderProgram::Bind() const
{
	glUseProgram(m_program);
}
//-----------------------------------------------------------------------
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
//-----------------------------------------------------------------------
void Shaders::UnBind()
{
	glUseProgram(0);
}
//-----------------------------------------------------------------------
Shaders::Shaders(const std::string &vertex, const std::string &frag)
{
	m_vs = new Shader(ShaderType::VERTEX, vertex);
	m_fs = new Shader(ShaderType::FRAGMENT, frag);
	m_program = new ShaderProgram(std::vector<Shader*>{ m_vs, m_fs });
}
//-----------------------------------------------------------------------
Shaders::~Shaders()
{
	delete m_program;
	delete m_vs;
	delete m_fs;
}
//-----------------------------------------------------------------------
void Shaders::Bind()
{
	m_program->Bind();
}
//-----------------------------------------------------------------------
void Shaders::Uniform1f(const char *var, float val)
{
	glUniform1f(getUniLoc(var), val);
}
//-----------------------------------------------------------------------
void Shaders::Uniform1i(const char *var, int val)
{
	glUniform1i(getUniLoc(var), val);
}
//-----------------------------------------------------------------------
void Shaders::Uniform1fv(const char *var, int count, float *vals)
{
	glUniform1fv(getUniLoc(var), count, vals);
}
//-----------------------------------------------------------------------
void Shaders::Uniform2f(const char *var, float v1, float v2)
{
	glUniform2f(getUniLoc(var), v1, v2);
}
//-----------------------------------------------------------------------
void Shaders::Uniform2fv(const char *var, int count, float *vals)
{
	glUniform2fv(getUniLoc(var), count, vals);
}
//-----------------------------------------------------------------------
void Shaders::UniformVec2(const char *var, const glm::vec2 &v)
{
	Uniform2f(var, v.x, v.y);
}
//-----------------------------------------------------------------------
void Shaders::Uniform3iv(const char *var, int count, int *vals)
{
	glUniform3iv(getUniLoc(var), count, vals);
}
//-----------------------------------------------------------------------
void Shaders::Uniform3fv(const char *var, int count, float *vals)
{
	glUniform3fv(getUniLoc(var), count, vals);
}
//-----------------------------------------------------------------------
void Shaders::Uniform3f(const char *var, const float v1, const float v2, const float v3)
{
	glUniform3f(getUniLoc(var), v1, v2, v3);
}
//-----------------------------------------------------------------------
void Shaders::UniformVec3(const char *var, const glm::vec3 &v)
{
	Uniform3f(var, v.x, v.y, v.z);
}
//-----------------------------------------------------------------------
void Shaders::UniformMatrix3fv(const char *var, GLsizei count, const GLfloat *vals, bool transpose)
{
	glUniformMatrix3fv(getUniLoc(var), count, transpose, vals);
}
//-----------------------------------------------------------------------
void Shaders::Uniform4iv(const char *var, int count, int *vals)
{
	glUniform4iv(getUniLoc(var), count, vals);
}
//-----------------------------------------------------------------------
void Shaders::Uniform4fv(const char *var, int count, float *vals)
{
	glUniform4fv(getUniLoc(var), count, vals);
}
//-----------------------------------------------------------------------
void Shaders::Uniform4f(const char *var, float v1, float v2, float v3, float v4)
{
	glUniform4f(getUniLoc(var), v1, v2, v3, v4);
}
//-----------------------------------------------------------------------
void Shaders::UniformVec4(const char *var, const glm::vec4 &v)
{
	Uniform4f(var, v.x, v.y, v.z, v.w);
}
//-----------------------------------------------------------------------
void Shaders::UniformMatrix4fv(const char *var, GLsizei count, const GLfloat *vals, bool transpose)
{
	glUniformMatrix4fv(getUniLoc(var), count, transpose, vals);
}
//-----------------------------------------------------------------------
GLint Shaders::getUniLoc(const char *name)
{
	const GLint loc = glGetUniformLocation(m_program->m_program, name);
	if ( loc == -1 )
		Log(LevelLog::Error) << "Shaders::getUniLoc(): uniform '" << name << "' has not been defined";
	return loc;
}
//-----------------------------------------------------------------------