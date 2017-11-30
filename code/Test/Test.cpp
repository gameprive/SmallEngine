#include "stdafx.h"
#include "Test.h"
#include "Renderer/Format.h"

//-----------------------------------------------------------------------
Test::Test(Window *window)
{
	const glm::vec3 data[] =
	{
		{ -1.0f, -1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
	};

	buffer = std::make_shared<VertexBuffer>(data, 3, sizeof(data), BufferUsage::Immutable);
	format = std::make_shared<VertexFormat>();
	format->PushAttribute(VectorType::Float3);
	format->Build(buffer);

	auto vs = std::make_shared<Shader>(ShaderType::Vertex, "data/shaders/test.vert");
	auto ps = std::make_shared<Shader>(ShaderType::Fragment, "data/shaders/test.frag");	
	shaders = std::make_shared<ShaderProgram>(std::vector<std::shared_ptr<Shader>>{ vs, ps });
}
//-----------------------------------------------------------------------
Test::~Test()
{
}
//-----------------------------------------------------------------------
void Test::Update(float dt)
{
}
//-----------------------------------------------------------------------
void Test::Render()
{
	format->Bind();
	shaders->Bind();
	glDrawArrays(GL_TRIANGLES, 0, buffer->GetVertexCount());
}
//-----------------------------------------------------------------------
void Test::Resize(int width, int height)
{
}
//-----------------------------------------------------------------------
bool Test::IsGameDone()
{
	return false;
}
//-----------------------------------------------------------------------