#include "stdafx.h"
#include "TestAnim.h"
#include "SkyBox.h"

//--------------------------------------------------------------------
bool TestAnim::Init()
{
	glEnable(GL_DEPTH_TEST);

	camera = Camera(glm::vec3(2.0f, 5.0f, 18.0f));

	shaders = std::make_shared<ShaderProgram>("shaders/animated_model.vert", "shaders/animated_model.frag");	

	model_astroboy.loadModel("models/man/model.dae");
	
	mat_model = glm::rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mat_model = glm::translate(mat_model, glm::vec3(3.0f, 0.0f, 0.0f));

	//SkyBox::Instance()->init("images/skybox_shadow");
	
	return true;
}
//--------------------------------------------------------------------
void TestAnim::Update(float dt)
{
	perspective_view = camera.getViewMatrix();
	perspective_projection = glm::perspective(glm::radians(camera.fov), (float)1024 / (float)768, 1.0f, 2000.0f);
	MVP = perspective_projection * perspective_view * mat_model;

	camera.updateKey(dt, 30);

	if ( Window::Get().GetMouseButton(GLFW_MOUSE_BUTTON_LEFT) )
	{
		double x, y;
		Window::Get().GetMousePos(x, y);
		Window::Get().SetCursorMode(GLFW_CURSOR_HIDDEN);

		if ( mouse_first_in )
		{
			last_x = x;
			last_y = y;
			mouse_first_in = false;
		}

		GLfloat x_offset = x - last_x;
		GLfloat y_offset = y - last_y;
		last_x = x;
		last_y = y;
		camera.updateMouse(x_offset, y_offset);
	}
	else
	{
		mouse_first_in = true;
		Window::Get().SetCursorMode(GLFW_CURSOR_NORMAL);
	}
		
	// delete translation from view matrix
	//SkyBox::Instance()->update(perspective_projection * glm::mat4(glm::mat3(perspective_view)));
}
//--------------------------------------------------------------------
void TestAnim::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	shaders->Bind();
	shaders->Uniform3("view_pos", camera.camera_pos);

	shaders->Uniform1("material.shininess", 32.0f);
	shaders->Uniform1("material.transparency", 1.0f);

	shaders->Uniform3("point_light.position", camera.camera_pos);
	shaders->Uniform3("point_light.ambient", 0.1f, 0.1f, 0.1f);
	shaders->Uniform3("point_light.diffuse", 1.0f, 1.0f, 1.0f);
	shaders->Uniform3("point_light.specular", 1.0f, 1.0f, 1.0f);
	shaders->Uniform1("point_light.constant", 1.0f);
	shaders->Uniform1("point_light.linear", 0.007f);
	shaders->Uniform1("point_light.quadratic", 0.0002f);

	shaders->UniformMatrix4("MVP", 1, glm::value_ptr(MVP));
	shaders->UniformMatrix4("M_matrix", 1, glm::value_ptr(mat_model));
	
	glm::mat4 matr_normals_cube2 = glm::mat4(glm::transpose(glm::inverse(mat_model)));
	shaders->UniformMatrix4("normals_matrix", 1, glm::value_ptr(matr_normals_cube2));

	model_astroboy.draw(shaders);

	//SkyBox::Instance()->draw();
}
//--------------------------------------------------------------------
void TestAnim::Close()
{
}
//--------------------------------------------------------------------