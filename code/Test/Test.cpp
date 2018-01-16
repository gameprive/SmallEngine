#include "stdafx.h"
#include "Test.h"
#include "Renderer/Format.h"

static const GLfloat VertexData[] =
{
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};

static const GLfloat uvData[] =
{
	0.000059f, 1.0f - 0.000004f,
	0.000103f, 1.0f - 0.336048f,
	0.335973f, 1.0f - 0.335903f,
	1.000023f, 1.0f - 0.000013f,
	0.667979f, 1.0f - 0.335851f,
	0.999958f, 1.0f - 0.336064f,
	0.667979f, 1.0f - 0.335851f,
	0.336024f, 1.0f - 0.671877f,
	0.667969f, 1.0f - 0.671889f,
	1.000023f, 1.0f - 0.000013f,
	0.668104f, 1.0f - 0.000013f,
	0.667979f, 1.0f - 0.335851f,
	0.000059f, 1.0f - 0.000004f,
	0.335973f, 1.0f - 0.335903f,
	0.336098f, 1.0f - 0.000071f,
	0.667979f, 1.0f - 0.335851f,
	0.335973f, 1.0f - 0.335903f,
	0.336024f, 1.0f - 0.671877f,
	1.000004f, 1.0f - 0.671847f,
	0.999958f, 1.0f - 0.336064f,
	0.667979f, 1.0f - 0.335851f,
	0.668104f, 1.0f - 0.000013f,
	0.335973f, 1.0f - 0.335903f,
	0.667979f, 1.0f - 0.335851f,
	0.335973f, 1.0f - 0.335903f,
	0.668104f, 1.0f - 0.000013f,
	0.336098f, 1.0f - 0.000071f,
	0.000103f, 1.0f - 0.336048f,
	0.000004f, 1.0f - 0.671870f,
	0.336024f, 1.0f - 0.671877f,
	0.000103f, 1.0f - 0.336048f,
	0.336024f, 1.0f - 0.671877f,
	0.335973f, 1.0f - 0.335903f,
	0.667969f, 1.0f - 0.671889f,
	1.000004f, 1.0f - 0.671847f,
	0.667979f, 1.0f - 0.335851f
};

glm::mat4 MVP;
//--------------------------------------------------------------------
bool Test::Init()
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 View = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 Model = glm::mat4(1.0f);
	MVP = Projection * View * Model;

	vertexbuffer = std::make_shared<VertexBuffer>(VertexData, 12 * 3, sizeof(VertexData), BufferUsage::Immutable);

	colorbuffer = std::make_shared<VertexBuffer>(uvData, 12 * 3, sizeof(uvData), BufferUsage::Immutable);

	format = std::make_shared<VertexFormat>();
	format->PushAttribute(VectorType::Float3);
	format->PushAttribute(VectorType::Float2);
	format->Build({ vertexbuffer, colorbuffer });

	shaders = std::make_shared<ShaderProgram>("data/shaders/test.vert", "data/shaders/test.frag");

	sampler = std::make_shared<SamplerState>(SamplerDescription::CreateAnisotropicClamp());
	//tex = std::make_shared<Texture2D>("data/textures/grass.png");
	tex = std::make_shared<Texture2D>("test_dxt1.dds");

	rt = std::make_shared<RenderTarget2D>(100, 100, 1, DataFormat::RGBA8, DepthFormat::Depth24Stencil8, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	return true;
}
//--------------------------------------------------------------------
void Test::Update(float dt)
{
}
//--------------------------------------------------------------------
void Test::Render()
{
	{
		RenderSystem::Get().SetRenderTarget2D(rt);
		RenderSystem::Get().Clear(glm::vec4(1.0f));

		shaders->Bind();
		shaders->UniformMatrix4("MVP", 1, glm::value_ptr(MVP));

		sampler->Bind(0);
		tex->Bind(0);
		format->Bind();

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	}
		
	{
		RenderSystem::Get().SetRenderTarget2D(nullptr);
		RenderSystem::Get().Clear(glm::vec4(0.5f));

		shaders->Bind();
		shaders->UniformMatrix4("MVP", 1, glm::value_ptr(MVP));

		sampler->Bind(0);
		rt->BindTexture(0);
		format->Bind();

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	}
}
//--------------------------------------------------------------------
void Test::Close()
{

}
//--------------------------------------------------------------------