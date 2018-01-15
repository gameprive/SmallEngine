#pragma once

#include "Shader.h"

class ShaderProgram : Noncopyable
{
public:
	ShaderProgram(const std::vector<std::shared_ptr<Shader>> &shaders);
	ShaderProgram(const std::string &vertexFilePath, const std::string &fragmentFilePath);
	~ShaderProgram();

	void Bind() const;

	void Uniform1i(const char*, int);
	void Uniform1f(const char*, float);
	void Uniform1fv(const char*, int, float*);
	void Uniform2f(const char*, float, float);
	void Uniform2fv(const char*, int, float*);
	void UniformVec2(const char*, const glm::vec2&);
	void Uniform3iv(const char*, int, int*);
	void Uniform3fv(const char *var, int count, const float *vals);
	void Uniform3f(const char*, const float, const float, const float);
	void UniformVec3(const char*, const glm::vec3&);
	void UniformMatrix3fv(const char*, int, const GLfloat*, bool = false);
	void Uniform4iv(const char*, int, int*);
	void Uniform4fv(const char *var, int count, const float *vals);
	void Uniform4f(const char*, float, float, float, float);
	void UniformVec4(const char*, const glm::vec4&);
	void UniformMatrix4fv(const char *var, int count, const GLfloat *vals, bool transpose = false);
	
private:
	void init(const std::vector<std::shared_ptr<Shader>> &shaders);
	void checkLinking(const GLuint program) const;
	GLint getUniLoc(const char*);
	void getUniformLocations();

	std::unordered_map<std::string, int> m_uniformLocations;

	GLuint m_program = 0;
};