#pragma once

#include "Renderer/Resource/VertexFormat.h"

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
};