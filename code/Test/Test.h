#pragma once

#include "Renderer/Resource/VertexFormat.h"
#include "Renderer/Resource/ShaderProgram.h"

class Test
{
public:
	Test(Window *window);
	~Test();

	void Update(float dt);
	void Render();
	void Resize(int width, int height);

	bool IsGameDone();
	
	std::shared_ptr<VertexBuffer> buffer;
	std::shared_ptr<VertexFormat> format;

	std::shared_ptr<ShaderProgram> shaders;
};