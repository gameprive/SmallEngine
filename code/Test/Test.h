#pragma once

#include "Renderer/Resource/VertexFormat.h"
#include "Renderer/Resource/Shader.h"

class Test
{
public:
	Test(Window *window);
	~Test();

	void Update(float dt);
	void Render();
	void Resize(int width, int height);

	bool IsGameDone();

	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

	std::shared_ptr<VertexBuffer> buffer;
	std::shared_ptr<VertexFormat> format;
};