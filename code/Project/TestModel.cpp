#include "stdafx.h"
#include "TestModel.h"
#include "LoadTexture.h"

//--------------------------------------------------------------------
TestModel::TestModel(const glm::vec3 &pos, float angle)
{
	setupModelMatrix(pos, angle);
	loadMesh();
	loadTextures();
	loadShader();
}
//--------------------------------------------------------------------
TestModel::~TestModel()
{
	glDeleteTextures(1, &m_diffuseMap);
	glDeleteTextures(1, &m_normalMap);
	glDeleteTextures(1, &m_specularMap);
	glDeleteTextures(1, &m_emissionMap);
	glDeleteBuffers(3, m_vbos);
	glDeleteVertexArrays(1, &m_vao);
}
//--------------------------------------------------------------------
void TestModel::Render(const glm::mat4 &projection, const glm::mat4 &view)
{
	const glm::mat3 normal = glm::inverseTranspose(glm::mat3(m_modelMat));

	m_shaders->Bind();
	m_shaders->UniformMatrix4fv("uProjection", 1, glm::value_ptr(projection));
	m_shaders->UniformMatrix4fv("uView", 1, glm::value_ptr(view));
	m_shaders->UniformMatrix4fv("uModel", 1, glm::value_ptr(m_modelMat));
	m_shaders->UniformMatrix3fv("uNormal", 1, glm::value_ptr(normal));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normalMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_specularMap);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_emissionMap);

	glDisable(GL_BLEND);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
}
//--------------------------------------------------------------------
void TestModel::setupModelMatrix(const glm::vec3 &pos, float angle)
{
	m_modelMat = glm::translate(pos);
	m_modelMat = glm::rotate(m_modelMat, angle, glm::vec3(0.0, 1.0, 0.0));
	m_modelMat = glm::scale(m_modelMat, glm::vec3(1.0));
}
//--------------------------------------------------------------------
void TestModel::loadMesh()
{
	GLMmodel *geometry = glmReadOBJ("Data/mesh/sign.obj");
	if ( geometry )
	{
		glmScale(geometry, 1.0f);
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glGenBuffers(3, m_vbos);
		glmBuildVBO(geometry, &m_numVertices, &m_vao, m_vbos);
	}
	glmDelete(geometry);
}
//--------------------------------------------------------------------
void TestModel::loadTextures()
{
	m_diffuseMap = LoadTexture("Data/textures/grass.png");
	m_normalMap = LoadTexture("Data/textures/sign-normal-map.png");
	m_specularMap = LoadTexture("Data/textures/sign-specular-map.png");
	m_emissionMap = LoadTexture("Data/textures/black.png");
}
//--------------------------------------------------------------------
void TestModel::loadShader()
{
	m_shaders = std::make_shared<ShaderProgram>("Data/shaders/solid.vert", "Data/shaders/solid.frag");
	m_shaders->Bind();
	m_shaders->Uniform1i("uDiffuseMap", 0);
	m_shaders->Uniform1i("uNormalMap", 1);
	m_shaders->Uniform1i("uSpecularMap", 2);
	m_shaders->Uniform1i("uEmissionMap", 3);
	m_shaders->UniformVec3("uMaterialDiffuse", glm::vec3(1.0, 1.0, 1.0));
	m_shaders->UniformVec3("uMaterialSpecular", glm::vec3(1.0, 1.0, 1.0));
	m_shaders->Uniform1f("uSpecularIntensity", 6.0);
	m_shaders->Uniform1f("uSpecularHardness", 16.0);
	m_shaders->Uniform1f("uNormalMapStrength", 1.5);
}
//--------------------------------------------------------------------
