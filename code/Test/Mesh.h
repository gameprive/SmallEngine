#pragma once

#include <assimp/Importer.hpp>

#define NUM_BONES_PER_VERTEX 4

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 text_coords;
};

struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

struct BoneMatrix
{
	aiMatrix4x4 offset_matrix;
	aiMatrix4x4 final_world_transform;

};

struct VertexBoneData
{
	uint32_t ids[NUM_BONES_PER_VERTEX];   // we have 4 bone ids for EACH vertex & 4 weights for EACH vertex
	float weights[NUM_BONES_PER_VERTEX];

	VertexBoneData()
	{
		memset(ids, 0, sizeof(ids));    // init all values in array = 0
		memset(weights, 0, sizeof(weights));
	}

	void addBoneData(uint32_t bone_id, float weight);
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertic, std::vector<GLuint> ind, std::vector<Texture> textur, std::vector<VertexBoneData> bone_id_weights);
	Mesh() {};
	~Mesh();

	// Render mesh
	void Draw(std::shared_ptr<ShaderProgram> shaders);

private:
	//Mesh data
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	std::vector<VertexBoneData> bones_id_weights_for_each_vertex;

	//buffers
	GLuint VAO;
	GLuint VBO_vertices;
	GLuint VBO_bones;
	GLuint EBO_indices;

	//inititalize buffers
	void SetupMesh();
};
