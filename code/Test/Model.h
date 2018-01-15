#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

class Model
{
public:
	Model();
	~Model();
	static const uint32_t MAX_BONES = 100;
	
	void initShaders(std::shared_ptr<ShaderProgram> shaders);
	void loadModel(const std::string& path);

	void draw(std::shared_ptr<ShaderProgram> shaders);

	void showNodeName(aiNode* node);

	glm::mat4 aiToGlm(aiMatrix4x4 ai_matr);
	aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend);

private:
	Assimp::Importer import;
	const aiScene* scene;
	std::vector<Mesh> meshes; // one mesh in one object
	std::string directory;

	std::map<std::string, uint32_t> m_bone_mapping; // maps a bone name and their index
	uint32_t m_num_bones = 0;
	std::vector<BoneMatrix> m_bone_matrices;
	aiMatrix4x4 m_global_inverse_transform;

	//GLuint m_bone_location[MAX_BONES];
	float ticks_per_second = 0.0f;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string type_name);


	uint32_t findPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
	uint32_t findRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
	uint32_t findScaling(float p_animation_time, const aiNodeAnim* p_node_anim);
	const aiNodeAnim* findNodeAnim(const aiAnimation* p_animation, const std::string p_node_name);
	// calculate transform matrix
	aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
	aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
	aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim);

	void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform);
	void boneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms);

	// rotate Head
	glm::quat rotate_head_xz = glm::quat(cos(glm::radians(0.0f)), sin(glm::radians(0.0f)) * glm::vec3(1.0f, 0.0f, 0.0f)); // this quad do nothingggggg!!!!!

};