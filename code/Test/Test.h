#pragma once

#include "Renderer/Resource/VertexBuffer.h"

class Test
{
public:
	Test(Window *window);
	~Test();

	void Update(float dt);
	void Render();
	void Resize(int width, int height);

	bool IsGameDone();

	std::unique_ptr<VertexBuffer> buffer;
};