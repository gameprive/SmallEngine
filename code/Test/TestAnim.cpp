#include "stdafx.h"
#include "TestAnim.h"
#include "SkyBox.h"
#include "ForShader.h"

//--------------------------------------------------------------------
bool TestAnim::Init()
{
	glEnable(GL_DEPTH_TEST);

	camera = Camera(glm::vec3(2.0f, 5.0f, 18.0f));

	shaders = std::make_shared<ShaderProgram>("shaders/animated_model.vert", "shaders/animated_model.frag");	
	//shaders = ForShader::makeProgram("shaders/animated_model.vert", "shaders/animated_model.frag");

	model_astroboy.initShaders(shaders);
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
	MVP = perspective_projection * perspective_view * mat_model;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	shaders->Bind();
	shaders->Uniform3f("view_pos", camera.camera_pos.x, camera.camera_pos.y, camera.camera_pos.z);

	shaders->Uniform1f("material.shininess", 32.0f);
	shaders->Uniform1f("material.transparency", 1.0f);

	shaders->Uniform3f("point_light.position", camera.camera_pos.x, camera.camera_pos.y, camera.camera_pos.z);
	shaders->Uniform3f("point_light.ambient", 0.1f, 0.1f, 0.1f);
	shaders->Uniform3f("point_light.diffuse", 1.0f, 1.0f, 1.0f);
	shaders->Uniform3f("point_light.specular", 1.0f, 1.0f, 1.0f);
	shaders->Uniform1f("point_light.constant", 1.0f);
	shaders->Uniform1f("point_light.linear", 0.007f);
	shaders->Uniform1f("point_light.quadratic", 0.0002f);

	shaders->UniformMatrix4fv("MVP", 1, glm::value_ptr(MVP));
	shaders->UniformMatrix4fv("M_matrix", 1, glm::value_ptr(mat_model));
	
	glm::mat4 matr_normals_cube2 = glm::mat4(glm::transpose(glm::inverse(mat_model)));
	shaders->UniformMatrix4fv("normals_matrix", 1, glm::value_ptr(matr_normals_cube2));

	//glUseProgram(shaders);

	//glUniform3f(glGetUniformLocation(shaders, "view_pos"), camera.camera_pos.x, camera.camera_pos.y, camera.camera_pos.z);
	
	//glUniform1f(glGetUniformLocation(shaders, "material.shininess"), 32.0f);
	//glUniform1f(glGetUniformLocation(shaders, "material.transparency"), 1.0f);
	
	// Point Light 1
	//glUniform3f(glGetUniformLocation(shaders, "point_light.position"), camera.camera_pos.x, camera.camera_pos.y, camera.camera_pos.z);
	//glUniform3f(glGetUniformLocation(shaders, "point_light.ambient"), 0.1f, 0.1f, 0.1f);
	//glUniform3f(glGetUniformLocation(shaders, "point_light.diffuse"), 1.0f, 1.0f, 1.0f);
	//glUniform3f(glGetUniformLocation(shaders, "point_light.specular"), 1.0f, 1.0f, 1.0f);
	//glUniform1f(glGetUniformLocation(shaders, "point_light.constant"), 1.0f);
	//glUniform1f(glGetUniformLocation(shaders, "point_light.linear"), 0.007);	//0.14 0.09  0.07  0.045  0.027  0.022  0.014  0.007  0.0014 -	������ ���������� ���������
	//glUniform1f(glGetUniformLocation(shaders, "point_light.quadratic"), 0.0002);//0.07 0.032 0.017 0.0075 0.0028 0.0019 0.0007 0.0002 0.000007	���������� -->
	
	//glUniformMatrix4fv(glGetUniformLocation(shaders, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	//glUniformMatrix4fv(glGetUniformLocation(shaders, "M_matrix"), 1, GL_FALSE, glm::value_ptr(mat_model));
	//glm::mat4 matr_normals_cube2 = glm::mat4(glm::transpose(glm::inverse(mat_model)));
	//glUniformMatrix4fv(glGetUniformLocation(shaders, "normals_matrix"), 1, GL_FALSE, glm::value_ptr(matr_normals_cube2));
	model_astroboy.draw(shaders);
//	glUseProgram(0);

	//SkyBox::Instance()->draw();
}
//--------------------------------------------------------------------
void TestAnim::Close()
{
//	glDeleteProgram(shaders);
}
//--------------------------------------------------------------------