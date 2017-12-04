#pragma once

class Shaders;

class TestModel
{
public:
	TestModel(const glm::vec3 &pos, float angle);
	~TestModel();

	void Render(const glm::mat4 &projection, const glm::mat4 &view);

private:
	void setupModelMatrix(const glm::vec3 &pos, float angle);
	void loadMesh();
	void loadTextures();
	void loadShader();

	glm::mat4 m_modelMat;

	std::shared_ptr<ShaderProgram> m_shaders;

	GLuint m_vao;
	GLuint m_vbos[3];
	int m_numVertices;

	GLuint m_diffuseMap;
	GLuint m_normalMap;
	GLuint m_specularMap;
	GLuint m_emissionMap;
};