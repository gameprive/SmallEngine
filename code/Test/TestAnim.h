#pragma once

#include "Camera.h"
#include "Model.h"

class TestAnim final : public IApp
{
public:
	bool Init() final;

	void Update(float dt) final;
	void Render() final;
	
	void Close() final;

private:
	std::shared_ptr<ShaderProgram> shaders;


	//GLuint shaders = 0;
	Model model_astroboy;
	glm::mat4 mat_model;
	Camera camera;

	glm::mat4 perspective_view;
	glm::mat4 perspective_projection;

	GLfloat last_x;
	GLfloat last_y;
	GLboolean mouse_first_in;
	
	glm::mat4 MVP;
};