#include "stdafx.h"
#include "Test.h"
#include "Renderer/Format.h"

GLuint Test::LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	auto vs = std::make_shared<Shader>(ShaderType::Vertex, vertex_file_path);
	auto ps = std::make_shared<Shader>(ShaderType::Fragment, fragment_file_path);
	
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vs->GetGlIdentifier());
	glAttachShader(ProgramID, ps->GetGlIdentifier());
	glLinkProgram(ProgramID);

	//// Check the program
	//glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	//glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	//if ( InfoLogLength > 0 )
	//{
	//	std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
	//	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	//	printf("%s\n", &ProgramErrorMessage[0]);
	//}

	glDetachShader(ProgramID, vs->GetGlIdentifier());
	glDetachShader(ProgramID, ps->GetGlIdentifier());
	
	return ProgramID;
}

GLuint programID;
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

	programID = LoadShaders("data/shaders/test.vert", "data/shaders/test.frag");
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
	glUseProgram(programID);
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