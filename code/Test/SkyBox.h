#pragma once

class SkyBox
{
public:
	static SkyBox* Instance()
	{
		static SkyBox inst;
		return &inst;
	}

	void init(char* folder);
	void update(glm::mat4 VP_matr);
	void draw();

private:
	~SkyBox();

	GLuint VAO_skybox;
	GLuint VBO_vertices_textures;

	GLuint cube_texture_id;
	GLuint createCubeTexture(char* folder);

	std::shared_ptr<ShaderProgram> shaders;

	glm::mat4 VP_matrix; // WITHOUT model for skybox
};