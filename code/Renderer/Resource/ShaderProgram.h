#pragma once

#include "Shader.h"

class ShaderProgram : Noncopyable
{
public:
	ShaderProgram(const std::vector<std::shared_ptr<Shader>> &shaders);
	ShaderProgram(const std::string &vertexFilePath, const std::string &fragmentFilePath);
	~ShaderProgram();

	void Bind() const;

	void Uniform1(const char *name, int);
	void Uniform1(const char *name, float);
	void Uniform2(const char *name, float v1, float v2);
	void Uniform2(const char *name, const glm::vec2 &v);
	void Uniform3(const char *name, float v1, float v2, float v3);
	void Uniform3(const char *name, const glm::vec3 &v);
	void Uniform4(const char *name, float v1, float v2, float v3, float v4);
	void Uniform4(const char *name, const glm::vec4 &v);
	
	void UniformMatrix3(const char *name, int count, const float *vals, bool transpose = false);
	void UniformMatrix4(const char *name, int count, const float *vals, bool transpose = false);

	void Uniform1v(const char *name, int count, const float *vals);
	void Uniform2v(const char *name, int count, const float *vals);
	void Uniform3v(const char *name, int count, const float *vals);	
	void Uniform4v(const char *name, int count, const float *vals);	
	
private:
	void init(const std::vector<std::shared_ptr<Shader>> &shaders);
	void checkLinking(const GLuint program) const;
	GLint getUniLoc(const char*);
	void getUniformLocations();

	std::unordered_map<std::string, int> m_uniformLocations;

	GLuint m_program = 0;
};