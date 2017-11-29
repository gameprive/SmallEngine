#pragma once

enum ShaderType
{
	VERTEX,
	FRAGMENT,
	TESSELLATION_CONTROL,
	TESSELLATION_EVALUATION,
	GEOMETRY
};

class Shader
{
public:
	Shader(ShaderType type, const std::string &path);
	~Shader();

	GLuint GetGlIdentifier() const { return m_shader; }
	ShaderType GetShaderType() const { return m_type; }

private:
	GLenum getGlShaderType(ShaderType type) const;
	void readFromFile(const std::string &path, std::string &out) const;
	void checkCompilation(const GLuint shader) const;

	GLuint m_shader;
	ShaderType m_type;
};

class ShaderProgram
{
	friend class Shaders;
public:
	ShaderProgram(const std::vector<Shader*> &shaders);
	~ShaderProgram();

	void Bind() const;

private:
	void checkLinking(const GLuint program) const;
	GLuint m_program;

	std::vector<Shader*> m_attachShader;
};

class Shaders
{
public:
	static void UnBind();

	Shaders(const std::string &vertexFile, const std::string &fragmentFile);
	~Shaders();

	void Bind();

	void Uniform1i(const char*, int);
	void Uniform1f(const char*, float);
	void Uniform1fv(const char*, int, float*);
	void Uniform2f(const char*, float, float);
	void Uniform2fv(const char*, int, float*);
	void UniformVec2(const char*, const glm::vec2&);
	void Uniform3iv(const char*, int, int*);
	void Uniform3fv(const char*, int, float*);
	void Uniform3f(const char*, const float, const float, const float);
	void UniformVec3(const char*, const glm::vec3&);
	void UniformMatrix3fv(const char*, int, const GLfloat*, bool = false);
	void Uniform4iv(const char*, int, int*);
	void Uniform4fv(const char*, int, float*);
	void Uniform4f(const char*, float, float, float, float);
	void UniformVec4(const char*, const glm::vec4&);
	void UniformMatrix4fv(const char*, int, const GLfloat*, bool = false);

private:
	GLint getUniLoc(const char*);

	ShaderProgram *m_program = nullptr;
	Shader *m_vs = nullptr;
	Shader *m_fs = nullptr;
};