// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "ShaderProgram.h"

//-----------------------------------------------------------------------
ShaderProgram::ShaderProgram(const std::vector<std::shared_ptr<Shader>> &shaders)
{
	m_program = glCreateProgram();
	
	for ( auto &s : shaders )
		glAttachShader(m_program, s->m_shader);
	
	glLinkProgram(m_program);
	checkLinking(m_program);
	
	for ( auto &s : shaders )
		glDetachShader(m_program, s->m_shader);
}
//-----------------------------------------------------------------------
ShaderProgram::~ShaderProgram()
{
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