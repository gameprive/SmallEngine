#pragma once

#include "Renderer/Resource/VertexFormat.h"
#include "Renderer/Resource/ShaderProgram.h"
#include "Renderer/Resource/SamplerState.h"
#include "Renderer/Resource/Texture2D.h"

class Test
{
public:
	Test(Window *window);
	~Test();

	void Update(float dt);
	void Render();
	void Resize(int width, int height);

	bool IsGameDone();
	
	std::shared_ptr<VertexBuffer> vertexbuffer;
	std::shared_ptr<VertexBuffer> colorbuffer;
	std::shared_ptr<VertexFormat> format;

	std::shared_ptr<ShaderProgram> shaders;

	std::shared_ptr<SamplerState> sampler;
	std::shared_ptr<Texture2D> tex;

};